
//const evtSource = new EventSource("http://192.168.0.71/events");
const evtSource = new EventSource("/events");
var lastReadingTime = 0;
var readsPerSecond = 0;

window.addEventListener('load', onload);
window.addEventListener('beforeunload', () => {
	evtSource.close();
});

function onload(event) {
    //initWebSocket();
    lastReading = Date.now();
}

evtSource.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);

evtSource.addEventListener("ping", function(e) {
  console.log("Ping received: " + e.data);
 }, false);

 evtSource.addEventListener("nodata", function(e) {
  console.log("AirBear cannot communicate with ECU");
 }, false);

// Function that receives the message from the ESP32 with the readings

evtSource.addEventListener("reading", function(e) {
  
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
  
  addLiveDataPoint(readsPerSecond.toFixed(2));
  document.getElementById('readSpeed').innerHTML = readsPerSecond.toFixed(2);

  
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
