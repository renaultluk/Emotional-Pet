#include "main.h"
#include <string>

const char* userID = "abc123";
const char* parentPath = "/users/" + userID;

void fcsUploadCallback(FCS_UploadStatusInfo info)
{
    if (info.status == fb_esp_fcs_upload_status_init)
    {
        Serial.printf("Uploading file %s (%d) to %s\n", info.localFileName.c_str(), info.fileSize, info.remoteFileName.c_str());
    }
    else if (info.status == fb_esp_fcs_upload_status_upload)
    {
        Serial.printf("Uploaded %d%s\n", (int)info.progress, "%");
    }
    else if (info.status == fb_esp_fcs_upload_status_complete)
    {
        Serial.println("Upload completed\n");
        FileMetaInfo meta = fbdo.metaData();
        Serial.printf("Name: %s\n", meta.name.c_str());
        Serial.printf("Bucket: %s\n", meta.bucket.c_str());
        Serial.printf("contentType: %s\n", meta.contentType.c_str());
        Serial.printf("Size: %d\n", meta.size);
        Serial.printf("Generation: %lu\n", meta.generation);
        Serial.printf("Metageneration: %lu\n", meta.metageneration);
        Serial.printf("ETag: %s\n", meta.etag.c_str());
        Serial.printf("CRC32: %s\n", meta.crc32.c_str());
        Serial.printf("Token: %s\n", meta.downloadTokens.c_str());
        Serial.printf("Download URL: %s\n\n", fbdo.downloadURL().c_str());
    }
    else if (info.status == fb_esp_fcs_upload_status_error)
    {
        Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
    }
}

void fcsDownloadCallback(FCS_DownloadStatusInfo info)
{
    if (info.status == fb_esp_fcs_download_status_init)
    {
        Serial.printf("Downloading file %s (%d) to %s\n", info.remoteFileName.c_str(), info.fileSize, info.localFileName.c_str());
    }
    else if (info.status == fb_esp_fcs_download_status_download)
    {
        Serial.printf("Downloaded %d%s\n", (int)info.progress, "%");
    }
    else if (info.status == fb_esp_fcs_download_status_complete)
    {
        Serial.println("Download completed\n");
    }
    else if (info.status == fb_esp_fcs_download_status_error)
    {
        Serial.printf("Download failed, %s\n", info.errorMsg.c_str());
    }
}

void streamCallback(FirebaseStream data)
{
  Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());
  printResult(data); // see addons/RTDBHelper.h
  Serial.println();

  Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());

  // Due to limited of stack memory, do not perform any task that used large memory here especially starting connect to server.
  // Just set this flag and check it status later.
  dataChanged = true;
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}

void firebaseInit()
{
  config.api_key = FB_API_KEY;
  config.database_url = FB_DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;

  Firebase.signUp(&config, &auth, "", "");
  Firebase.begin(&config, &auth);

  config.fcs.upload_buffer_size = 512;

  if (!Firebase.RTDB.beginStream(&stream, "/test/stream/data"))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
}

void uploadFile(String path)
{
  if (!Firebase.Storage.upload(&fbdo, FB_STORAGE_BUCKET_ID /* Firebase Storage bucket id */, path /* path to local file */, mem_storage_type_sd /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, "media.wav" /* path of remote file stored in the bucket */, "audio/x-wav" /* mime type */, fcsUploadCallback /* callback function */))
    Serial.println(fbdo.errorReason());
}

void downloadFile(String remote_path, String local_path)
{
  if (!Firebase.Storage.download(&fbdo, FB_STORAGE_BUCKET_ID /* Firebase Storage bucket id */, remote_path /* path of remote file stored in the bucket */, local_path /* path to local file */, mem_storage_type_sd /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */))
    Serial.println(fbdo.errorReason());
}

void checkFirebaseDataChanged()
{
  if (dataChanged)
  {
    dataChanged = false;
    // When stream data is available, do anything here...
  }
}

void addFriend(int friendID)
{
/*
    friendList[friendListPointer] = friendID; //add friend element into the array
    friendListPointer ++;
    
    if Firebase.RTDB,setArray(&fbdo, "/users/userId/friends", friendList)) { //upload and check if elements are uploaded successfully into Firebase
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
    }
*/
}

void getFriends()
{
/*
    if (Firebase.RTDB.getArray(&fbdo, "/users/userId/friends")) { //if there's data
    friendList = fbdo.jsonArray();
    }
    else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
    }
*/
}

void getAudio(String path)
{

}

void postAudio()
{

}

void getEmotion()
{
/*
     if (Firebase.RTDB.getString(&fbdo, "/users/userId/emotion")) { //**the address is not set
     if (fbdo.dataType() == "string") {
        return fbdo.stringData();
      }
    }
    else {
      return;
    }
*/
}
