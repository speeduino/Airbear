//Data structure arrays go here
var livechart;
function initLiveChart()
{
    var options = {
        type: 'line',
        data: {

          datasets: [
              {
                label: 'Data Rate',
                data: [],
              }
            ]
        },
        options: {
          responsive: false,
          animation: false,
          plugins: {
            legend: {
              display: false
            },
            tooltip: {
              enabled: false
            },
          },
          elements: {
            line: {
              backgroundColor: '#999999',
              borderColor: '#000000',
              borderWidth: 0.3,
              //tension: 1,
              fill: false
            },
            point: {
              radius: 0
            }
          },
          scales: {
            x: {
                type: 'realtime',
                display: false,
                realtime: {
                  duration: 10000,
                  delay: 1000,
                }
              },
            y: {
              display: true,
              min: 0,
              max: 150,
              reverse: false,
              title: {
                display: true,
                text: 'Data Rate/s'
              }
            }
          }
        }
      }

      var ctx = document.getElementById('liveChart').getContext('2d');
      livechart = new Chart(ctx, options);
}

function addLiveDataPoint(reading) 
{
  livechart.data.datasets[0].data.push({
    x: Date.now(),
    y: reading
  });
  //livechart.update('quiet');
  //console.log("Adding live data point")
}
initLiveChart();