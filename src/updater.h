
#ifndef UPDATER_H
#define UPDATER_H

#define UPDATE_REMOTE_URL  "/updateFWRemote"
#define UPDATE_DATA_UPLOAD_URL  "/updateDataUpload"
#define UPDATE_FW_UPLOAD_URL  "/updateFWUpload"

#include "globals.h"

void partitionUploadChunk(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final, uint8_t partitionType);
void partitionUploadComplete(AsyncWebServerRequest *request);
String saveRemoteFW_URLs(AsyncWebServerRequest *request);
void updateFromRemote();
String update_progress_json(AsyncWebServerRequest *request);
bool updatesPending();
String updateInProgressPage();

#endif