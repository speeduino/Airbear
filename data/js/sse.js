
//const evtSource = new EventSource("http://192.168.0.71/events");
const evtSource = new EventSource("/events");
var lastReadingTime = 0;
var readsPerSecond = 0;
// Connection state variables
let isConnected = false;
let isSimulating = false;
let simulationInterval;
let lastValues = {
  rpm: 1000,
  TPS: 0,
  CLT: 80,
  IAT: 25,
  PW1: 2.5,
  MAP: 100,
  AFR1: 14.7
};

// Update connection UI
function updateConnectionUI() {
  const indicator = document.getElementById('connection-indicator');
  const text = document.getElementById('connection-text');
  const toggle = document.getElementById('simulation-toggle');
  
  if (isConnected) {
    indicator.className = 'connected';
    text.textContent = 'Connected';
    toggle.style.display = 'none';
  } else {
    indicator.className = 'disconnected';
    text.textContent = 'Disconnected';
    toggle.style.display = 'block';
  }
}

// Generate simulated data
function generateSimulatedData() {
  // More realistic simulation with different behaviors for each gauge
  const now = Date.now();
  
  // RPM - simulates engine revving up and down
  lastValues.rpm = Math.max(800, Math.min(8000, 
    lastValues.rpm + (Math.random() * 200 - 100) + 
    (300 * Math.sin(now/5000))));
  
  // TPS - simulates throttle input
  lastValues.TPS = Math.max(0, Math.min(100, 
    lastValues.TPS + (Math.random() * 5 - 2.5) + 
    (20 * Math.sin(now/7000))));
  
  // CLT - slowly oscillating coolant temp
  lastValues.CLT = 80 + (15 * Math.sin(now/60000));
  
  // IAT - faster changing intake temp
  lastValues.IAT = 25 + (10 * Math.sin(now/30000));
  
  // PW1 - correlates with RPM and TPS
  lastValues.PW1 = Math.max(1, Math.min(20, 
    1 + (lastValues.rpm/1000) * 0.5 + (lastValues.TPS/100) * 3));
  
  // MAP - correlates with RPM and TPS
  lastValues.MAP = Math.max(30, Math.min(250, 
    30 + (lastValues.rpm/50) + (lastValues.TPS * 1.5)));
  
  // AFR - oscillates around stoichiometric
  lastValues.AFR1 = 14.7 + (0.5 * Math.sin(now/4000));
  
  // Mark as simulated data
  return {...lastValues, __isSimulated: true};
}

// Start/stop simulation
function toggleSimulation(enable) {
  const checkbox = document.getElementById('enable-simulation');
  
  if (enable) {
    isSimulating = true;
    checkbox.checked = true;
    
    // Start with current values or defaults
    if (!lastValues) {
      lastValues = {
        rpm: 1000,
        TPS: 0,
        CLT: 80,
        IAT: 25,
        PW1: 2.5,
        MAP: 100,
        AFR1: 14.7
      };
    }
    
    // First immediate update
    const simulatedData = generateSimulatedData();
    window.updateGaugesWithConfiguration(simulatedData);
    
    // Regular updates
    simulationInterval = setInterval(() => {
      const simulatedData = generateSimulatedData();
      window.updateGaugesWithConfiguration(simulatedData);
    }, 33); // ~30Hz
    
    console.log("Simulation started");
  } else {
    isSimulating = false;
    checkbox.checked = false;
    clearInterval(simulationInterval);
    console.log("Simulation stopped");
  }
}

// Initialize connection monitoring
function initConnectionMonitor() {
  updateConnectionUI();
  
  // Setup simulation toggle
  const checkbox = document.getElementById('enable-simulation');
  if (checkbox) {
    checkbox.addEventListener('change', (e) => {
      toggleSimulation(e.target.checked);
    });
  }
  
  // Check if we should start in simulation mode (for testing)
  const urlParams = new URLSearchParams(window.location.search);
  if (urlParams.has('simulate')) {
    toggleSimulation(true);
  }
}

document.addEventListener('DOMContentLoaded', initConnectionMonitor);
window.addEventListener('load', onload);
window.addEventListener('beforeunload', () => {
	evtSource.close();
});

function onload(event) {
    //initWebSocket();
    lastReading = Date.now();
}

evtSource.addEventListener('open', function(e) {
  isConnected = true;
  updateConnectionUI();
  if (isSimulating) toggleSimulation(false);
 }, false);

 evtSource.addEventListener('error', function(e) {
  isConnected = false;
  updateConnectionUI();
  
  // If we were connected before, try to reconnect
  if (e.target.readyState === EventSource.CLOSED) {
    setTimeout(() => {
      if (!isConnected && !isSimulating) {
        location.reload();
      }
    }, 5000);
  }
}, false);

evtSource.addEventListener("ping", function(e) {
  console.log("Ping received: " + e.data);
 }, false);

 evtSource.addEventListener("nodata", function(e) {
  console.log("AirBear cannot communicate with ECU");
 }, false);

// Function that receives the message from the ESP32 with the readings
evtSource.addEventListener("reading", function(e) {

  isConnected = true;
  updateConnectionUI();
  
  if (isSimulating) {
    toggleSimulation(false);
    document.getElementById('enable-simulation').checked = false;
  }
  
  //console.log("ECU frame received");
  var arrivedTime = Date.now();
  //var arrivedTime = e.timeStamp;
  var frameTime = (arrivedTime - lastReadingTime)
  if(frameTime < 10)
  {
    //console.log("Too soon to process reading");
    return;
  }
  //console.log(e.data);

  if(arrivedTime > lastReadingTime)
  {
    var tempReadsPerSecond = Math.round( (1000 / frameTime), 0);
    readsPerSecond = smooth(tempReadsPerSecond, 0.3, readsPerSecond);
    lastReadingTime = arrivedTime;
  }

  var ecuDataFrame = JSON.parse(e.data);
  if (window.updateGaugesWithConfiguration) {
    window.updateGaugesWithConfiguration(ecuDataFrame);
  }
  // Store last values in case we need to simulate
  lastValues = {...ecuDataFrame};
  return;

  gauges[0].value = ecuDataFrame['rpm'];
  gauges[0].valueText = Math.round(ecuDataFrame['rpm']);

  var TPS_val = Math.round(ecuDataFrame['TPS']/2)
  gauges[1].value = TPS_val;
  document.getElementById("gauge2-value").innerText = TPS_val;

  gauges[2].value = ecuDataFrame['CLT'];
  document.getElementById("gauge3-value").innerText = Math.round(ecuDataFrame['CLT']);

  gauges[3].value = ecuDataFrame['IAT'];
  document.getElementById("gauge4-value").innerText = Math.round(ecuDataFrame['IAT']);

  gauges[4].value = ecuDataFrame['PW'];
  document.getElementById("gauge5-value").innerText = Math.round(ecuDataFrame['PW1']);

  gauges[5].value = ecuDataFrame['MAP'];
  document.getElementById("gauge6-value").innerText = Math.round(ecuDataFrame['MAP']);

  gauges[6].value = ecuDataFrame['AFR1'];
  document.getElementById("gauge7-value").innerText = ecuDataFrame['AFR1'];
  //gauges[2].valueText = Math.round(ecuDataFrame['IAT']);


  //gauges[3].valueText = Math.round(ecuDataFrame['CLT']);
  //gauges[3].value = ecuDataFrame['secl'];
  
  //addLiveDataPoint(readsPerSecond.toFixed(2));
  //document.getElementById('readSpeed').innerHTML = readsPerSecond.toFixed(2);

  
}, false);

function updateGauges()
{
  for(var i = 0; i < gauges.length; i++)
  {
    gauges[i].update();
  }
}
if (navigator.userAgent.indexOf("Firefox") != -1)
{
  //https://github.com/Mikhus/canvas-gauges/issues/251
  setInterval(updateGauges, 1000);
}


function smooth(newVal, filterVal, currentVal){


  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .99;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }

  var smoothedVal = (newVal * (1 - filterVal)) + (currentVal  *  filterVal);

  return smoothedVal;
}