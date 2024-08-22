
var gaugeConfig_rpm = {
    type: "radial-gauge",
    width: 400,
    height: 400,
    units: "RPM",
    title: false,
    value: 0,
    minValue: 0,
    maxValue: 9000,
    majorTicks: ["0","1000","2000","3000","4000","5000","6000","7000","8000","9000"], 
    minorTicks: 2,
    strokeTicks: false,
    highlights: [
        //{ "from": 0, "to": 5000, "color": "rgba(0,255,0,.15)" },
        { "from": 5000, "to": 6500, "color": "rgba(255,255,0,.15)" },
        { "from": 6500, "to": 9000, "color": "rgba(255,30,0,.25)" },
    ],
    colorPlate: "#555",
    colorPlateEnd: "#111",
    //colorPlate: "rgba(255, 255, 255, 0)",
    colorMajorTicks: "#f5f5f5",
    colorMinorTicks: "#ddd",
    colorTitle: "#fff",
    colorUnits: "#ccc",
    colorNumbers: "#eee",
    colorNeedleStart: "rgba(240, 128, 128, 1)",
    colorNeedleEnd: "rgba(255, 160, 122, .9)",
    colorBorderOuterEnd: "rgba(255, 160, 122, .5)",
    valueBox: true,
    valueInt: 4,
    valueDec: 0,
    animation: false,
    animationRule: "linear",
    animationDuration: "33", //Must be at most 1000/data frequency
    animatedValue: true,
    fontValue: "Led",
}

var gaugeConfig_rpm2 = {
  type: "radial-gauge",
  width: 400,
  height: 400,
  units: "RPM",
  title: "x100",
  fontTitle: "Orbitron",
  value: 0,
  minValue: 0,
  maxValue: 9000,
  //majorTicks: ["0","1000","2000","3000","4000","5000","6000","7000","8000","9000"], 
  majorTicks: ["0","10","20","30","40","50","60","70","80","90"], 
  minorTicks: 2,
  strokeTicks: false,
  highlights: [
      //{ "from": 0, "to": 5000, "color": "rgba(0,255,0,.15)" },
      { "from": 5000, "to": 6500, "color": "rgba(255,255,0,.2)" },
      { "from": 6500, "to": 9000, "color": "rgba(255,30,0,.5)" },
  ],
  colorPlate: "#111",
  colorPlateEnd: "#137",
  colorMajorTicks: "#f5f5f5",
  colorMinorTicks: "#ddd",
  colorTitle: "#fff",
  colorUnits: "#ccc",
  colorNumbers: "#eee",
  colorNeedleStart: "rgba(255, 0, 0, 1)",
  colorNeedleEnd: "rgba(255, 0, 0, .9)",
  colorNeedleShadowDown: "rgba(255, 255, 255, 0.5)",
  colorBorderOuterEnd: "rgba(255, 160, 122, .5)",
  colorBorderInner: "#137",
  colorBorderMiddle: "#137",
  valueBox: true,
  valueInt: 4,
  valueDec: 0,
  animation: false,
  animationRule: "linear",
  animationDuration: "33", //Must be at most 1000/data frequency
  animatedValue: true,
  //fontValue: "Led",
  fontValue: "Orbitron",
  fontUnits: "Orbitron",
  fontNumbers: "Orbitron",
  fontNumbersStyle: "italic",
  fontNumbersSize: 16,
}

var gaugeConfig_linear_generic = {
  type: "linear-gauge",
  width: 275,
  height: 120,
  units: "",
  title: false,
  value: 75,
  valueBox: false,
  minValue: 0,
  maxValue: 100,
  colorPlate: "rgba(255, 255, 255, 0)",
  borderShadowWidth: 0,
  borders: false,
  barBeginCircle: false,
  tickSide: "left",
  numberSide: "left",
  needleSide: "left",
  needleType: "line",
  needleWidth: 4,
  needleStart: -20,
  needleEnd: 160,
  colorNeedle: "rgb(252, 200, 78)",
  colorNeedleEnd: "rgba(252, 200, 78, 0)",
  animation: false,
  animationDuration: 30,
  animationRule: "linear",
  animationTarget: "plate",
  barWidth: 50,
  colorBar: "rgba(255, 255, 255, 0)",
  colorBarProgress: "rgba(26, 49, 110, 1)",
  ticksWidth: 0,
  ticksWidthMinor: 0,
  majorTicksInt: 1,
  highlightsWidth: 0,
  fontNumbersSize: 0,
}

var gaugeConfig_qtrRight = {
    width: 300,
    height: 300,
    units: "C",
    title: "CLT",
    minValue: 0,
    startAngle: 180,
    ticksAngle: 90,
    valueBox: false,
    maxValue: 220,
    majorTicks: [
        "-40",
        "-20",
        "0",
        "20",
        "40",
        "60",
        "80",
        "100",
        "120",
        "140"
    ],
    minorTicks: 2,
    strokeTicks: true,
    highlights: [
        {
            "from": 160,
            "to": 220,
            "color": "rgba(200, 50, 50, .75)"
        }
    ],
    colorPlate: "rgba(255, 255, 255, 0)",
    borderShadowWidth: 0,
    borders: false,
    needleType: "arrow",
    needleWidth: 2,
    needleCircleSize: 7,
    needleCircleOuter: true,
    needleCircleInner: false,
    //animation: false,
    animationDuration: 15,
    animationRule: "linear",
    valueBox: true,
};

var gaugeConfig_qtrLeft = {
    width: 300,
    height: 300,
    units: "C",
    title: "IAT",
    minValue: 0,
    startAngle: 90,
    ticksAngle: 90,
    barStartPosition: "right",
    valueBox: false,
    maxValue: 90,
    majorTicks: 
    [
        "140",
        "120",
        "100",
        "90",
        "70",
        "40",
        "20",
        "0",
        "-20",
        "-40"
    ],
    minorTicks: 2,
    strokeTicks: true,
    highlights: [
        {
            "from": 0,
            "to": 25,
            "color": "rgba(200, 50, 50, .75)"
        }
    ],
    colorPlate: "rgba(255, 255, 255, 0)",
    borderShadowWidth: 0,
    borders: false,
    needleType: "arrow",
    needleWidth: 2,
    needleCircleSize: 7,
    needleCircleOuter: true,
    needleCircleInner: false,
    //animation: false,
    animationDuration: 15,
    animationRule: "linear",
    valueBox: true,
};
const gauges = [];

gauges.push(new RadialGauge(Object.assign( {renderTo: 'gauge1' }, gaugeConfig_rpm2)));
gauges[0].update({value: 0, maxValue: 9000});
gauges[0].draw();

gauges.push(new LinearGauge(Object.assign( {renderTo: 'gauge2' }, gaugeConfig_linear_generic)));
gauges[1].update({value: 0, maxValue: 100});
gauges[1].draw();
gauges.push(new LinearGauge(Object.assign( {renderTo: 'gauge3' }, gaugeConfig_linear_generic)));
gauges[2].update({minValue: -40, maxValue: 180});
gauges[2].draw();
gauges.push(new LinearGauge(Object.assign( {renderTo: 'gauge4' }, gaugeConfig_linear_generic)));
gauges[3].draw();

gauges.push(new LinearGauge(Object.assign( {renderTo: 'gauge5' }, gaugeConfig_linear_generic)));
gauges[4].update({tickSide: "right"});
gauges[4].draw();
gauges.push(new LinearGauge(Object.assign( {renderTo: 'gauge6' }, gaugeConfig_linear_generic)));
gauges[5].update({tickSide: "right", maxValue: 250})
gauges[5].draw();
gauges.push(new LinearGauge(Object.assign( {renderTo: 'gauge7' }, gaugeConfig_linear_generic)));
gauges[6].update({tickSide: "right"})
gauges[6].draw();