#ifndef OTA_PAGE_H
#define OTA_PAGE_H

#include <Arduino.h>

// OTA Upload Page
const char otaPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>AlphaSign ESP32 - OTA Update</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      max-width: 600px;
      margin: 50px auto;
      padding: 20px;
      background: linear-gradient(to bottom, #2c3e50, #34495e);
      color: #ecf0f1;
    }
    h1 {
      color: #3498db;
      text-align: center;
      text-shadow: 2px 2px 4px rgba(0,0,0,0.5);
    }
    .container {
      background: #34495e;
      padding: 30px;
      border-radius: 10px;
      box-shadow: 0 4px 6px rgba(0,0,0,0.3);
    }
    form {
      margin: 20px 0;
    }
    input[type="file"] {
      width: 100%;
      padding: 10px;
      margin: 10px 0;
      border: 2px dashed #3498db;
      border-radius: 5px;
      background: #2c3e50;
      color: #ecf0f1;
      cursor: pointer;
    }
    input[type="submit"] {
      background: #3498db;
      color: white;
      padding: 12px 30px;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      font-size: 16px;
      width: 100%;
      transition: background 0.3s;
    }
    input[type="submit"]:hover {
      background: #2980b9;
    }
    #progress {
      margin-top: 20px;
      padding: 15px;
      border-radius: 5px;
      background: #2c3e50;
      border-left: 4px solid #3498db;
      display: none;
    }
    .progress-bar {
      width: 100%;
      height: 30px;
      background: #2c3e50;
      border-radius: 5px;
      overflow: hidden;
      margin-top: 10px;
    }
    .progress-fill {
      height: 100%;
      background: linear-gradient(90deg, #27ae60, #2ecc71);
      width: 0%;
      transition: width 0.3s;
      display: flex;
      align-items: center;
      justify-content: center;
      color: white;
      font-weight: bold;
    }
    .back-link {
      display: block;
      text-align: center;
      margin-top: 20px;
      color: #3498db;
      text-decoration: none;
    }
    .back-link:hover {
      text-decoration: underline;
    }
    .info {
      background: #2c3e50;
      padding: 15px;
      border-radius: 5px;
      margin-bottom: 20px;
      border-left: 4px solid #e67e22;
    }
  </style>
  <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>
</head>
<body>
  <h1>🔄 OTA Firmware Update</h1>
  <div class="container">
    <div class="info">
      <strong>⚠️ Instructions:</strong>
      <ul style="margin: 10px 0;">
        <li>Export .bin file: Arduino IDE → Sketch → Export Compiled Binary</li>
        <li>Select the .bin file below and click Update</li>
        <li>Wait for upload to complete (device will reboot automatically)</li>
        <li>Do not disconnect power during update!</li>
      </ul>
    </div>
    
    <form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>
      <input type='file' name='update' accept='.bin' required>
      <input type='submit' value='Update Firmware'>
    </form>
    
    <div id='progress'>
      <div>Upload Progress:</div>
      <div class="progress-bar">
        <div class="progress-fill" id="progress-fill">0%</div>
      </div>
      <div id="status" style="margin-top: 10px;"></div>
    </div>
    
    <a href="/" class="back-link">← Back to Control Panel</a>
  </div>
  
  <script>
    $('form').submit(function(e) {
      e.preventDefault();
      var form = $('#upload_form')[0];
      var data = new FormData(form);
      
      $('#progress').show();
      $('#status').html('Uploading firmware...');
      
      $.ajax({
        url: '/ota',
        type: 'POST',
        data: data,
        contentType: false,
        processData: false,
        xhr: function() {
          var xhr = new window.XMLHttpRequest();
          xhr.upload.addEventListener('progress', function(evt) {
            if (evt.lengthComputable) {
              var per = evt.loaded / evt.total;
              var percent = Math.round(per * 100);
              $('#progress-fill').css('width', percent + '%');
              $('#progress-fill').html(percent + '%');
            }
          }, false);
          return xhr;
        },
        success: function(d, s) {
          $('#status').html('✅ Upload successful! Device is rebooting...<br>Please wait 10-20 seconds then refresh the page.');
          setTimeout(function() {
            window.location.href = '/';
          }, 15000);
        },
        error: function(a, b, c) {
          $('#status').html('❌ Upload failed: ' + a.responseText);
        }
      });
    });
  </script>
</body>
</html>
)rawliteral";

#endif // OTA_PAGE_H
