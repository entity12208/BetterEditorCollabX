#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/GameObject.hpp>
#include <json/json.h>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <boost/asio.hpp>
#include <webrtc/api/peer_connection_interface.h>
#include <webrtc/api/create_peerconnection_factory.h>
#include <thread>
#include <iostream>
#include <map>

using namespace geode::prelude;
using websocketpp::connection_hdl;

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef std::map<connection_hdl, std::shared_ptr<LevelEditorLayer>> connection_map;

client ws_client;
connection_map editors;

class EditorCollab : public LevelEditorLayer {
    void setupRealtimeCollab();
    void handleCollabMessage(const std::string& message);
    void sendUpdateMessage(const std::string& message);

    void onAddObject(GameObject* obj);
    void onRemoveObject(GameObject* obj);
    void onUpdateObjectProperties(GameObject* obj);
    void onLevelSave();

    connection_hdl conn_hdl;
};

void EditorCollab::setupRealtimeCollab() {
    ws_client.clear_access_channels(websocketpp::log::alevel::all);
    ws_client.init_asio();

    ws_client.set_open_handler([this](connection_hdl hdl) {
        this->conn_hdl = hdl;
        editors[hdl] = this;
    });

    ws_client.set_message_handler([this](connection_hdl hdl, client::message_ptr msg) {
        this->handleCollabMessage(msg->get_payload());
    });

    websocketpp::lib::error_code ec;
    client::connection_ptr con = ws_client.get_connection("ws://localhost:9002", ec);
    if (ec) {
        std::cerr << "Could not create connection: " << ec.message() << std::endl;
    }

    ws_client.connect(con);
    std::thread([this]() { ws_client.run(); }).detach();
}

void EditorCollab::handleCollabMessage(const std::string& message) {
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(message, root)) {
        std::string type = root["type"].asString();
        if (type == "addObject") {
            // Handle addObject message
        } else if (type == "removeObject") {
            // Handle removeObject message
        } else if (type == "updateObjectProperties") {
            // Handle updateObjectProperties message
        }
    }
}

void EditorCollab::sendUpdateMessage(const std::string& message) {
    ws_client.send(conn_hdl, message, websocketpp::frame::opcode::text);
}

void EditorCollab::onAddObject(GameObject* obj) {
    Json::Value root;
    root["type"] = "addObject";
    root["id"] = obj->getID();
    Json::StreamWriterBuilder writer;
    sendUpdateMessage(Json::writeString(writer, root));
}

void EditorCollab::onRemoveObject(GameObject* obj) {
    Json::Value root;
    root["type"] = "removeObject";
    root["id"] = obj->getID();
    Json::StreamWriterBuilder writer;
    sendUpdateMessage(Json::writeString(writer, root));
}

void EditorCollab::onUpdateObjectProperties(GameObject* obj) {
    Json::Value root;
    root["type"] = "updateObjectProperties";
    root["id"] = obj->getID();
    Json::StreamWriterBuilder writer;
    sendUpdateMessage(Json::writeString(writer, root));
}

void EditorCollab::onLevelSave() {
    Json::Value root;
    root["type"] = "levelSave";
    Json::StreamWriterBuilder writer;
    sendUpdateMessage(Json::writeString(writer, root));
}

class $modify(GameObject) {
    void setPosition(cocos2d::CCPoint const& pos) {
        GameObject::setPosition(pos);
        if (LevelEditorLayer::get() != nullptr) {
            static_cast<EditorCollab*>(LevelEditorLayer::get())->onUpdateObjectProperties(this);
        }
    }
};

class $modify(LevelEditorLayer) {
    void init(GJGameLevel* lvl) {
        LevelEditorLayer::init(lvl);
        static_cast<EditorCollab*>(this)->setupRealtimeCollab();
    }
    
    bool addObject(GameObject* obj) {
        if (LevelEditorLayer::addObject(obj)) {
            static_cast<EditorCollab*>(this)->onAddObject(obj);
            return true;
        }
        return false;
    }
    
    void removeObject(GameObject* obj, bool undo) {
        LevelEditorLayer::removeObject(obj, undo);
        static_cast<EditorCollab*>(this)->onRemoveObject(obj);
    }
    
    void saveLevel() {
        LevelEditorLayer::saveLevel();
        static_cast<EditorCollab*>(this)->onLevelSave();
    }
};
