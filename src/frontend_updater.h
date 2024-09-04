
#ifndef FRONTEND_UPDATER_H
#define FRONTEND_UPDATER_H

#define UPDATE_DATA_REMOTE_URL  "/updateDataRemote"
#define UPDATE_DATA_UPLOAD_URL  "/updateDataUpload"
#define UPDATE_FW_REMOTE_URL  "/updateFWRemote"
#define UPDATE_FW_UPLOAD_URL  "/updateFWUpload"

#include "globals.h"

String dataPartitionDownload(AsyncWebServerRequest *request, uint8_t partitionType);
void partitionUploadChunk(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final, uint8_t partitionType);
void partitionUploadComplete(AsyncWebServerRequest *request);

#endif