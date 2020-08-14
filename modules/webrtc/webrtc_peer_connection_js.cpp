/*************************************************************************/
/*  webrtc_peer_connection_js.cpp                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifdef JAVASCRIPT_ENABLED

#include "webrtc_peer_connection_js.h"

#include "webrtc_data_channel_js.h"

#include "platform/javascript/audio_driver_javascript.h"
#include "core/io/json.h"
#include "emscripten.h"

extern "C" {
EMSCRIPTEN_KEEPALIVE void _emrtc_on_ice_candidate(void *obj, char *p_MidName, int p_MlineIndexName, char *p_sdpName) {
	WebRTCPeerConnectionJS *peer = static_cast<WebRTCPeerConnectionJS *>(obj);
	peer->emit_signal("ice_candidate_created", String(p_MidName), p_MlineIndexName, String(p_sdpName));
}

EMSCRIPTEN_KEEPALIVE void _emrtc_session_description_created(void *obj, char *p_type, char *p_offer) {
	WebRTCPeerConnectionJS *peer = static_cast<WebRTCPeerConnectionJS *>(obj);
	peer->emit_signal("session_description_created", String(p_type), String(p_offer));
}

EMSCRIPTEN_KEEPALIVE void _emrtc_on_connection_state_changed(void *obj) {
	WebRTCPeerConnectionJS *peer = static_cast<WebRTCPeerConnectionJS *>(obj);
	peer->_on_connection_state_changed();
}

EMSCRIPTEN_KEEPALIVE void _emrtc_on_error() {
	ERR_PRINT("RTCPeerConnection error!");
}

EMSCRIPTEN_KEEPALIVE void _emrtc_emit_channel(void *obj, int p_id) {
	WebRTCPeerConnectionJS *peer = static_cast<WebRTCPeerConnectionJS *>(obj);
	peer->emit_signal("data_channel_received", Ref<WebRTCDataChannelJS>(new WebRTCDataChannelJS(p_id)));
}

EMSCRIPTEN_KEEPALIVE const float* _emrtc_get_stream_buffer(void *obj) {
	AudioEffectRecord *record = static_cast<AudioEffectRecord *>(obj);
	int _size = 0;
	return record->get_recording_raw(_size);
}

EMSCRIPTEN_KEEPALIVE int _emrtc_get_stream_buffer_size(void *obj) {
	AudioEffectRecord *record = static_cast<AudioEffectRecord *>(obj);
	int _size = 0;
	record->get_recording_raw(_size);
	return _size;
}

EMSCRIPTEN_KEEPALIVE void _emrtc_reset_stream_buffer(void *obj) {
	AudioEffectRecord *record = static_cast<AudioEffectRecord *>(obj);
	record->set_recording_active(false);
	record->set_recording_active(true);
}

EMSCRIPTEN_KEEPALIVE void _emrtc_emit_stream(void *obj, int p_id) {
	WebRTCPeerConnectionJS *peer = static_cast<WebRTCPeerConnectionJS *>(obj);

	Ref<AudioStreamGenerator> track;
	track.instance();
	track->connect("playback_instanced", peer, "_track_instanced", varray(p_id));

	peer->emit_signal("media_track_received", track);
}

EMSCRIPTEN_KEEPALIVE void* _emrtc_get_playback(void* obj, int p_id) {
	WebRTCPeerConnectionJS *peer = static_cast<WebRTCPeerConnectionJS *>(obj);
	Map<int, ObjectID>::Element *el = peer->_playbacks.find(p_id);
	if (el) {
		AudioStreamGeneratorPlayback *playback = Object::cast_to<AudioStreamGeneratorPlayback>(ObjectDB::get_instance(el->get()));
		if (playback == NULL) {
			peer->_playbacks.erase(el);
		}
		return playback;
	}
	return NULL;
}

EMSCRIPTEN_KEEPALIVE void _emrtc_playback_push_frame(void *obj, float p_left, float p_right) {
	AudioStreamGeneratorPlayback *playback = static_cast<AudioStreamGeneratorPlayback *>(obj);
	playback->push_frame(AudioFrame(p_left, p_right));
}
}

void _emrtc_create_pc(int p_id, const Dictionary &p_config) {
	String config = JSON::print(p_config);
	/* clang-format off */
	EM_ASM({
		var dict = Module.IDHandler.get($0);
		var c_ptr = dict["ptr"];
		var config = JSON.parse(UTF8ToString($1));
		// Setup local connaction
		var conn = null;
		try {
			conn = new RTCPeerConnection(config);
		} catch (e) {
			console.log(e);
			return;
		}
		conn.oniceconnectionstatechange = function(event) {
			if (!Module.IDHandler.get($0)) return;
			ccall("_emrtc_on_connection_state_changed", "void", ["number"], [c_ptr]);
		};
		conn.onicecandidate = function(event) {
			if (!Module.IDHandler.get($0)) return;
			if (!event.candidate) return;

			var c = event.candidate;
			// should emit on ice candidate
			ccall("_emrtc_on_ice_candidate",
				"void",
				["number", "string", "number", "string"],
				[c_ptr, c.sdpMid, c.sdpMLineIndex, c.candidate]
			);
		};
		conn.ondatachannel = function (evt) {
			var dict = Module.IDHandler.get($0);
			if (!dict) {
				return;
			}
			var id = Module.IDHandler.add({"channel": evt.channel, "ptr": null});
			ccall("_emrtc_emit_channel",
				"void",
				["number", "number"],
				[c_ptr, id]
			);
		};
		conn.ontrack = function (evt) {
			var dict = Module.IDHandler.get($0);
			if (!dict) {
				return;
			}
			var stream = new MediaStream();
			stream.addTrack(evt.track);
			var id = Module.IDHandler.add({"stream": stream});
			ccall("_emrtc_emit_stream",
				"void",
				["number", "number"],
				[c_ptr, id]
			);
		};
		dict["conn"] = conn;
	}, p_id, config.utf8().get_data());
	/* clang-format on */
}

void WebRTCPeerConnectionJS::_on_connection_state_changed() {
	/* clang-format off */
	_conn_state = (ConnectionState)EM_ASM_INT({
		var dict = Module.IDHandler.get($0);
		if (!dict) return 5; // CLOSED
		var conn = dict["conn"];
		switch(conn.iceConnectionState) {
			case "new":
				return 0;
			case "checking":
				return 1;
			case "connected":
			case "completed":
				return 2;
			case "disconnected":
				return 3;
			case "failed":
				return 4;
			case "closed":
				return 5;
		}
		return 5; // CLOSED
	}, _js_id);
	/* clang-format on */
}

void WebRTCPeerConnectionJS::_track_instanced(Ref<AudioStreamGeneratorPlayback> p_playback, int p_js_id) {
	/* clang-format off */
	int id = EM_ASM_INT({
		try {
			var dict = Module.IDHandler.get($0);
			var track = Module.IDHandler.get($1);
			var driver = Module.IDHandler.get($2);
			if (!dict || !driver) return;

			var jsId;

			var source = driver["context"].createMediaStreamSource(track["stream"]);
			var script = driver["context"].createScriptProcessor(driver["script"].bufferSize, 2, 2);
			var getPlayback = cwrap("_emrtc_get_playback", "number", ["number"]);
			var playbackPushFrame = cwrap("_emrtc_playback_push_frame", null, ["number", "number", "number"]);
			script.onaudioprocess = function(audioProcessingEvent) {
				var playback = getPlayback(dict["ptr"], jsId);
				if (playback == 0) {
					Module.IDHandler.remove(jsId);
					source.disconnect(script);
					source = undefined;
					script = undefined;
					return;
				}
				var input = audioProcessingEvent.inputBuffer;
				var inputDataL = input.getChannelData(0);
				var inputDataR = input.numberOfChannels > 1 ? input.getChannelData(1) : inputDataL;
				for (var i = 0; i < inputDataL.length; i++) {
					playbackPushFrame(playback, inputDataL[i], inputDataR[i]);
				}
			};

			source.connect(script);

			jsId = Module.IDHandler.add({
				"script": script,
				"source": source
			});
			return jsId;
		} catch (e) {
			console.log(e);
			return 0;
		}
	}, _js_id, p_js_id, AudioDriverJavaScript::singleton->get_js_driver_id());
	/* clang-format on */

	_playbacks[id] = p_playback->get_instance_id();
}

void WebRTCPeerConnectionJS::close() {
	/* clang-format off */
	EM_ASM({
		var dict = Module.IDHandler.get($0);
		if (!dict) return;
		if (dict["conn"]) {
			dict["conn"].close();
		}
	}, _js_id);
	/* clang-format on */
	_conn_state = STATE_CLOSED;
}

Error WebRTCPeerConnectionJS::create_offer() {
	ERR_FAIL_COND_V(_conn_state != STATE_NEW, FAILED);

	_conn_state = STATE_CONNECTING;
	/* clang-format off */
	EM_ASM({
		var dict = Module.IDHandler.get($0);
		var conn = dict["conn"];
		var c_ptr = dict["ptr"];
		var onError = function(error) {
			console.error(error);
			ccall("_emrtc_on_error", "void", [], []);
		};
		var onCreated = function(offer) {
			ccall("_emrtc_session_description_created",
				"void",
				["number", "string", "string"],
				[c_ptr, offer.type, offer.sdp]
			);
		};
		conn.createOffer().then(onCreated).catch(onError);
	}, _js_id);
	/* clang-format on */
	return OK;
}

Error WebRTCPeerConnectionJS::set_local_description(String type, String sdp) {
	/* clang-format off */
	EM_ASM({
		var dict = Module.IDHandler.get($0);
		var conn = dict["conn"];
		var c_ptr = dict["ptr"];
		var type = UTF8ToString($1);
		var sdp = UTF8ToString($2);
		var onError = function(error) {
			console.error(error);
			ccall("_emrtc_on_error", "void", [], []);
		};
		conn.setLocalDescription({
			"sdp": sdp,
			"type": type
		}).catch(onError);
	}, _js_id, type.utf8().get_data(), sdp.utf8().get_data());
	/* clang-format on */
	return OK;
}

Error WebRTCPeerConnectionJS::set_remote_description(String type, String sdp) {
	if (type == "offer") {
		ERR_FAIL_COND_V(_conn_state != STATE_NEW, FAILED);
		_conn_state = STATE_CONNECTING;
	}
	/* clang-format off */
	EM_ASM({
		var dict = Module.IDHandler.get($0);
		var conn = dict["conn"];
		var c_ptr = dict["ptr"];
		var type = UTF8ToString($1);
		var sdp = UTF8ToString($2);

		var onError = function(error) {
			console.error(error);
			ccall("_emrtc_on_error", "void", [], []);
		};
		var onCreated = function(offer) {
			ccall("_emrtc_session_description_created",
				"void",
				["number", "string", "string"],
				[c_ptr, offer.type, offer.sdp]
			);
		};
		var onSet = function() {
			if (type != "offer") {
				return;
			}
			conn.createAnswer().then(onCreated);
		};
		conn.setRemoteDescription({
			"sdp": sdp,
			"type": type
		}).then(onSet).catch(onError);
	}, _js_id, type.utf8().get_data(), sdp.utf8().get_data());
	/* clang-format on */
	return OK;
}

Error WebRTCPeerConnectionJS::add_ice_candidate(String sdpMidName, int sdpMlineIndexName, String sdpName) {
	/* clang-format off */
	EM_ASM({
		var dict = Module.IDHandler.get($0);
		var conn = dict["conn"];
		var c_ptr = dict["ptr"];
		var sdpMidName = UTF8ToString($1);
		var sdpMlineIndexName = UTF8ToString($2);
		var sdpName = UTF8ToString($3);
		conn.addIceCandidate(new RTCIceCandidate({
			"candidate": sdpName,
			"sdpMid": sdpMidName,
			"sdpMlineIndex": sdpMlineIndexName
		}));
	}, _js_id, sdpMidName.utf8().get_data(), sdpMlineIndexName, sdpName.utf8().get_data());
	/* clang-format on */
	return OK;
}

Error WebRTCPeerConnectionJS::initialize(Dictionary p_config) {
	_emrtc_create_pc(_js_id, p_config);
	return OK;
}

Ref<WebRTCDataChannel> WebRTCPeerConnectionJS::create_data_channel(String p_channel, Dictionary p_channel_config) {
	String config = JSON::print(p_channel_config);
	/* clang-format off */
	int id = EM_ASM_INT({
		try {
			var dict = Module.IDHandler.get($0);
			if (!dict) return 0;
			var label = UTF8ToString($1);
			var config = JSON.parse(UTF8ToString($2));
			var conn = dict["conn"];
			return Module.IDHandler.add({
				"channel": conn.createDataChannel(label, config),
				"ptr": null
			})
		} catch (e) {
			console.log(e);
			return 0;
		}
	}, _js_id, p_channel.utf8().get_data(), config.utf8().get_data());
	/* clang-format on */
	ERR_FAIL_COND_V(id == 0, NULL);
	return memnew(WebRTCDataChannelJS(id));
}

Error WebRTCPeerConnectionJS::add_track(Ref<AudioEffectRecord> p_source) {
	if (_tracks.has(p_source)) return ERR_ALREADY_IN_USE;

	AudioEffectRecord *ptr = p_source.ptr();

	/* clang-format off */
	int track_id = EM_ASM_INT({
		try {
			var dict = Module.IDHandler.get($0);
			var driver = Module.IDHandler.get($1);
			var record = $2;
			if (!dict || !driver) return;

			var dest = driver["context"].createMediaStreamDestination(driver["context"], {});
			var script = driver["context"].createScriptProcessor(driver["script"].bufferSize, 2, 2);
			var streamGetBuffer = cwrap("_emrtc_get_stream_buffer", "number", ["number"]);
			var streamGetBufferSize = cwrap("_emrtc_get_stream_buffer_size", "number", ["number"]);
			var streamResetBuffer = cwrap("_emrtc_reset_stream_buffer", null, ["number"]);
			var numberOfChannels = 2;
			script.onaudioprocess = function(audioProcessingEvent) {
				var output = audioProcessingEvent.outputBuffer;

				var source = streamGetBuffer(record);
				var sourceSize = streamGetBufferSize(record);

				//if (sourceSize != output.length) console.log("Buffer underflow/overflow: Output data size mismatch!", sourceSize, output.length);

				var internalBuffer = HEAPF32.subarray(
						source / HEAPF32.BYTES_PER_ELEMENT,
						source / HEAPF32.BYTES_PER_ELEMENT + sourceSize * numberOfChannels);

				for (var channel = 0; channel < output.numberOfChannels; channel++) {
					var outputData = output.getChannelData(channel);
					// Loop through samples.
					for (var sample = 0; sample < Math.min(outputData.length, sourceSize); sample++) {
						outputData[sample] = internalBuffer[sample * numberOfChannels + channel % numberOfChannels];
					}
					for (var sample = Math.min(outputData.length, sourceSize); sample < outputData.length; sample++) {
						outputData[sample] = 0;
					}
				}
				streamResetBuffer(record);
			};

			script.connect(dest);
			driver["script"].connect(script); // Hopefully ensure that our audio process is called after the main audio process

			var tracks = dest.stream.getTracks();
			for (var i = 0; i < tracks.length; i++) {
				dict["conn"].addTrack(tracks[i], dest.stream);
			}

			return Module.IDHandler.add({
				"dest": dest,
				"script": script
			});
		} catch (e) {
			console.log(e);
			return 0;
		}
	}, _js_id, AudioDriverJavaScript::singleton->get_js_driver_id(), ptr);
	/* clang-format on */

	p_source->set_recording_active(true);
	_tracks[p_source] = track_id;

	return OK;
};

void WebRTCPeerConnectionJS::remove_track(Ref<AudioEffectRecord> p_source) {
	if (!_tracks.has(p_source)) return;

	/* clang-format off */
	EM_ASM({
		var dict = Module.IDHandler.get($0);
		var driver = Module.IDHandler.get($1);
		var stream = Module.IDHandler.get($2);
		Module.IDHandler.remove($2);
		if (!dict || !driver || !stream) return;

		stream["script"].onaudioprocess = null;

		stream["script"].disconnect(stream["dest"]);
		driver["script"].disconnect(stream["script"]);

		var tracks = stream["dest"].stream.getTracks();
		for (var i = 0; i < tracks.length; i++) {
			dict["conn"].removeTrack(tracks[i], dest.stream);
		}

		stream["script"] = undefined;
		stream["dest"] = undefined;
	}, _js_id, AudioDriverJavaScript::singleton->get_js_driver_id(), _tracks[p_source]);
	/* clang-format on */

	_tracks.erase(p_source);

};

Error WebRTCPeerConnectionJS::poll() {
	return OK;
}

WebRTCPeerConnection::ConnectionState WebRTCPeerConnectionJS::get_connection_state() const {
	return _conn_state;
}

void WebRTCPeerConnectionJS::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_track_instanced"), &WebRTCPeerConnectionJS::_track_instanced);
}

WebRTCPeerConnectionJS::WebRTCPeerConnectionJS() {
	_conn_state = STATE_NEW;

	/* clang-format off */
	_js_id = EM_ASM_INT({
		return Module.IDHandler.add({"conn": null, "ptr": $0});
	}, this);
	/* clang-format on */
	Dictionary config;
	_emrtc_create_pc(_js_id, config);
}

WebRTCPeerConnectionJS::~WebRTCPeerConnectionJS() {
	close();
	/* clang-format off */
	EM_ASM({
		Module.IDHandler.remove($0);
	}, _js_id);
	/* clang-format on */
};
#endif
