#ifndef HOME_PAGE_H
#define HOME_PAGE_H

#include <Arduino.h>

// HTML content for the web interface
const char homePage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>AlphaSign ESP32 Control</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      max-width: 800px;
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
    .tabs {
      display: flex;
      gap: 10px;
      margin-bottom: 20px;
      border-bottom: 2px solid #2c3e50;
    }
    .tab-button {
      background: #2c3e50;
      color: #95a5a6;
      padding: 12px 24px;
      border: none;
      border-radius: 5px 5px 0 0;
      cursor: pointer;
      font-size: 16px;
      font-weight: bold;
      transition: all 0.3s;
      flex: 1;
    }
    .tab-button:hover {
      background: #34495e;
      color: #ecf0f1;
    }
    .tab-button.active {
      background: #3498db;
      color: white;
    }
    .tab-content {
      display: none;
    }
    .tab-content.active {
      display: block;
    }
    .form-group {
      margin-bottom: 20px;
    }
    label {
      display: block;
      margin-bottom: 5px;
      font-weight: bold;
      color: #3498db;
    }
    input, select, textarea {
      width: 100%;
      padding: 10px;
      border: none;
      border-radius: 5px;
      box-sizing: border-box;
      font-size: 16px;
      background: #2c3e50;
      color: #ecf0f1;
    }
    textarea {
      resize: vertical;
      min-height: 80px;
    }
    button {
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
    button:hover {
      background: #2980b9;
    }
    .power-control {
      margin-bottom: 30px;
      padding: 20px;
      background: #2c3e50;
      border-radius: 5px;
      text-align: center;
    }
    .power-button {
      width: 120px;
      height: 120px;
      border-radius: 50%;
      border: none;
      font-size: 20px;
      font-weight: bold;
      cursor: pointer;
      transition: all 0.3s;
      box-shadow: 0 4px 8px rgba(0,0,0,0.3);
    }
    .power-button.off {
      background: linear-gradient(145deg, #e74c3c, #c0392b);
      color: white;
    }
    .power-button.off:hover {
      background: linear-gradient(145deg, #c0392b, #a93226);
      box-shadow: 0 6px 12px rgba(0,0,0,0.4);
    }
    .power-button.on {
      background: linear-gradient(145deg, #27ae60, #229954);
      color: white;
    }
    .power-button.on:hover {
      background: linear-gradient(145deg, #229954, #1e8449);
      box-shadow: 0 6px 12px rgba(0,0,0,0.4);
    }
    .admin-buttons {
      padding: 20px;
      background: #2c3e50;
      border-radius: 5px;
      text-align: center;
      margin-bottom: 20px;
    }
    .admin-buttons button {
      margin-top: 15px;
      padding: 10px 20px;
      font-size: 14px;
      font-weight: bold;
      width: auto;
      min-width: 180px;
    }
    .admin-buttons p {
      margin-top: 5px;
      margin-bottom: 0;
      font-size: 12px;
      color: #95a5a6;
    }
    .status {
      padding: 15px;
      border-radius: 5px;
      background: #2c3e50;
      border-left: 4px solid #3498db;
    }
    .info-grid {
      display: grid;
      grid-template-columns: auto 1fr;
      gap: 10px;
      margin-top: 10px;
    }
    .info-label {
      color: #3498db;
      font-weight: bold;
    }
    .ota-link {
      display: block;
      margin-top: 20px;
      padding: 15px;
      background: #34495e;
      border-radius: 5px;
      text-align: center;
      text-decoration: none;
      color: #3498db;
      border: 2px solid #3498db;
      font-weight: bold;
      transition: all 0.3s;
    }
    .ota-link:hover {
      background: #3498db;
      color: white;
    }
  </style>
</head>
<body>
  <h1>AlphaSign ESP32 Control</h1>
  <div class="container">
    <div class="tabs">
      <button class="tab-button active" onclick="switchTab('control')">Control</button>
      <button class="tab-button" onclick="switchTab('admin')">Admin</button>
    </div>
    
    <div id="control-tab" class="tab-content active">
      <div class="power-control">
        <h3>Sign Power Control</h3>
        <button id="powerButton" class="power-button off" onclick="togglePower()">OFF</button>
        <p id="powerStatus" style="margin-top: 15px; color: #e74c3c; font-weight: bold;">Power: OFF</p>
      </div>
      
      <form id="signForm" onsubmit="sendMessage(event)">
        <div class="form-group">
          <label for="message">Message:</label>
          <textarea id="message" name="message" placeholder="Enter your message...">Hello ESP32!</textarea>
        </div>
      
      <div class="form-group">
        <label for="file">Text File:</label>
        <select id="file" name="file">
          <option value="A" selected>A</option>
          <option value="B">B</option>
          <option value="C">C</option>
          <option value="D">D</option>
          <option value="E">E</option>
          <option value="F">F</option>
          <option value="G">G</option>
          <option value="H">H</option>
          <option value="I">I</option>
          <option value="J">J</option>
          <option value="K">K</option>
          <option value="L">L</option>
          <option value="M">M</option>
          <option value="N">N</option>
          <option value="O">O</option>
          <option value="P">P</option>
          <option value="Q">Q</option>
          <option value="R">R</option>
          <option value="S">S</option>
          <option value="T">T</option>
          <option value="U">U</option>
          <option value="V">V</option>
          <option value="W">W</option>
          <option value="X">X</option>
          <option value="Y">Y</option>
          <option value="Z">Z</option>
        </select>
      </div>
      
      <div class="form-group">
        <label for="mode">Display Mode:</label>
        <select id="mode" name="mode">
          <optgroup label="Standard Modes">
            <option value="scroll" selected>Scroll</option>
            <option value="hold">Hold</option>
            <option value="rotate">Rotate</option>
            <option value="flash">Flash</option>
            <option value="rollup">Roll Up</option>
            <option value="rolldown">Roll Down</option>
            <option value="rollleft">Roll Left</option>
            <option value="rollright">Roll Right</option>
            <option value="wipeup">Wipe Up</option>
            <option value="wipedown">Wipe Down</option>
            <option value="wipeleft">Wipe Left</option>
            <option value="wiperight">Wipe Right</option>
            <option value="automode">Auto Mode</option>
            <option value="rollin">Roll In</option>
            <option value="rollout">Roll Out</option>
            <option value="wipein">Wipe In</option>
            <option value="wipeout">Wipe Out</option>
            <option value="compressedrotate">Compressed Rotate</option>
            <option value="explode">Explode</option>
            <option value="clock">Clock</option>
          </optgroup>
          <optgroup label="Special Effects">
            <option value="twinkle">Twinkle</option>
            <option value="sparkle">Sparkle</option>
            <option value="snow">Snow</option>
            <option value="interlock">Interlock</option>
            <option value="switch">Switch</option>
            <option value="slide">Slide</option>
            <option value="spray">Spray</option>
            <option value="starburst">Starburst</option>
            <option value="welcome">Welcome</option>
            <option value="slotmachine">Slot Machine</option>
            <option value="newsflash">News Flash</option>
            <option value="trumpetanimation">Trumpet Animation</option>
            <option value="cyclecolors">Cycle Colors</option>
          </optgroup>
          <optgroup label="Graphics Animations">
            <option value="thankyou">Thank You</option>
            <option value="nosmoking">No Smoking</option>
            <option value="dontdrinkdrive">Don't Drink & Drive</option>
            <option value="runninganimal">Running Animal</option>
            <option value="fireworks">Fireworks</option>
            <option value="turbocar">Turbo Car</option>
            <option value="cherrybomb">Cherry Bomb</option>
          </optgroup>
        </select>
      </div>
      
      <div class="form-group">
        <label for="color">Color:</label>
        <select id="color" name="color">
          <option value="red">Red</option>
          <option value="green">Green</option>
          <option value="amber">Amber</option>
          <option value="orange">Orange</option>
          <option value="yellow">Yellow</option>
          <option value="rainbow1">Rainbow 1</option>
          <option value="rainbow2">Rainbow 2</option>
        </select>
      </div>
      
      <div class="form-group">
        <label for="speed">Speed:</label>
        <select id="speed" name="speed">
          <option value="1">Speed 1 (Slowest)</option>
          <option value="2">Speed 2</option>
          <option value="3" selected>Speed 3</option>
          <option value="4">Speed 4</option>
          <option value="5">Speed 5 (Fastest)</option>
        </select>
      </div>
      
      <button type="submit">Update Sign</button>
      </form>
    </div>
    
    <div id="admin-tab" class="tab-content">
      <div class="admin-buttons">
        <h3>Sign Management</h3>
        
        <button onclick="setRunSequence()" style="background: #2ecc71;">▶️ Set Run Sequence</button>
        <p>Enable rotation of all files with content</p>
        
        <button onclick="initializeFiles()" style="background: #e67e22;">🔧 Allocate Files</button>
        <p>Allocate A-Z (keeps existing content)</p>
        
        <button onclick="clearMemory()" style="background: #e74c3c;">🗑️ Clear Memory</button>
        <p>⚠️ ERASES all messages!</p>
      </div>
      
      <div class="status">
        <h3>System Status</h3>
        <div class="info-grid">
          <span class="info-label">IP Address:</span>
          <span id="ipAddress">Loading...</span>
          <span class="info-label">WiFi Signal:</span>
          <span id="rssi">Loading...</span>
          <span class="info-label">Uptime:</span>
          <span id="uptime">Loading...</span>
          <span class="info-label">Free Heap:</span>
          <span id="heap">Loading...</span>
        </div>
      </div>
      
      <a href="/ota" class="ota-link">🔄 Firmware Update (OTA)</a>
    </div>
  </div>
  
  <script>
    // Load system status on page load
    window.onload = function() {
      updateStatus();
      updatePowerState();
      setInterval(updateStatus, 5000); // Update every 5 seconds
    };
    
    function switchTab(tabName) {
      // Hide all tabs
      const tabs = document.getElementsByClassName('tab-content');
      for (let i = 0; i < tabs.length; i++) {
        tabs[i].classList.remove('active');
      }
      
      // Remove active class from all tab buttons
      const tabButtons = document.getElementsByClassName('tab-button');
      for (let i = 0; i < tabButtons.length; i++) {
        tabButtons[i].classList.remove('active');
      }
      
      // Show selected tab
      document.getElementById(tabName + '-tab').classList.add('active');
      
      // Highlight active tab button
      event.target.classList.add('active');
    };
    
    function updateStatus() {
      fetch('/status')
        .then(response => response.json())
        .then(data => {
          document.getElementById('ipAddress').textContent = data.ip;
          document.getElementById('rssi').textContent = data.rssi + ' dBm';
          document.getElementById('uptime').textContent = data.uptime;
          document.getElementById('heap').textContent = data.heap + ' bytes';
        })
        .catch(error => console.error('Error:', error));
    }
    
    function sendMessage(event) {
      event.preventDefault();
      
      const formData = new FormData(event.target);
      const params = new URLSearchParams(formData);
      
      fetch('/update?' + params.toString())
        .then(response => response.text())
        .then(data => {
          alert(data);
        })
        .catch(error => {
          alert('Error: ' + error);
        });
    }
    
    function togglePower() {
      fetch('/power/toggle')
        .then(response => response.json())
        .then(data => {
          updatePowerButton(data.state);
        })
        .catch(error => {
          console.error('Error:', error);
          alert('Failed to toggle power');
        });
    }
    
    function updatePowerState() {
      fetch('/power/state')
        .then(response => response.json())
        .then(data => {
          updatePowerButton(data.state);
        })
        .catch(error => console.error('Error:', error));
    }
    
    function initializeFiles() {
      if (!confirm('Allocate files A-Z?\n\nThis will allocate memory for files A-Z.\nExisting content is preserved.\n\nMake sure the sign is powered ON!')) {
        return;
      }
      
      fetch('/files/init')
        .then(response => response.json())
        .then(data => {
          if (data.success) {
            alert('✅ Files A-Z allocated!\n\nExisting content preserved.\nYou can now update files and set run sequence.');
          } else {
            alert('❌ Failed to allocate files');
          }
        })
        .catch(error => {
          console.error('Error:', error);
          alert('❌ Error allocating files: ' + error);
        });
    }
    
    function clearMemory() {
      if (!confirm('⚠️ CLEAR ALL SIGN MEMORY?\n\nThis will ERASE all messages from the sign!\nYou will need to re-send all your messages.\n\nAre you sure?')) {
        return;
      }
      
      fetch('/memory/clear')
        .then(response => response.json())
        .then(data => {
          if (data.success) {
            alert('✅ Memory cleared!\n\nAll messages erased.\nClick "Allocate Files" then update your messages.');
          } else {
            alert('❌ Failed to clear memory');
          }
        })
        .catch(error => {
          console.error('Error:', error);
          alert('❌ Error clearing memory: ' + error);
        });
    }
    
    function setRunSequence() {
      if (!confirm('Set run sequence to A-Z?\n\nThis will enable rotation of ALL files.\nOnly files with content will actually display.')) {
        return;
      }
      
      fetch('/runsequence/reset')
        .then(response => response.json())
        .then(data => {
          if (data.success) {
            alert('✅ ' + data.message);
          } else {
            alert('❌ Failed to set run sequence');
          }
        })
        .catch(error => {
          console.error('Error:', error);
          alert('❌ Error setting run sequence: ' + error);
        });
    }
    
    function updatePowerButton(state) {
      const button = document.getElementById('powerButton');
      const status = document.getElementById('powerStatus');
      
      if (state) {
        button.classList.remove('off');
        button.classList.add('on');
        button.textContent = 'ON';
        status.textContent = 'Power: ON';
        status.style.color = '#27ae60';
      } else {
        button.classList.remove('on');
        button.classList.add('off');
        button.textContent = 'OFF';
        status.textContent = 'Power: OFF';
        status.style.color = '#e74c3c';
      }
    }
  </script>
</body>
</html>
)rawliteral";

#endif // HOME_PAGE_H