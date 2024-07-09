#ifndef BETTER_EDITOR_COLLAB_H
#define BETTER_EDITOR_COLLAB_H

#include <string>
#include <vector>

// Define the VoiceChatHandler class
class VoiceChatHandler {
public:
    VoiceChatHandler();
    ~VoiceChatHandler();

    void startVoiceChat();
    void stopVoiceChat();
    void processVoiceData();

private:
    // Add necessary private members
};

// Define the UserSession class
class UserSession {
public:
    UserSession();
    ~UserSession();

    void startSession();
    void endSession();
    void syncWithServer();
    void handleCollab(std::vector<std::string>& collabData);

private:
    // Add necessary private members
};

// Define any other necessary classes or functions
void updateLevelProperties();

#endif // BETTER_EDITOR_COLLAB_H
