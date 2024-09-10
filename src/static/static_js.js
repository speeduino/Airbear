//This file is uglified using UglifyJS 3 and the config file within this directory.
//The output of this is placed into static_js.cpp
//This can be generaetd with the cmd: uglifyjs --config-file uglifyjs.config.json static_js.js

function getElementByID(id)
{
  return document.getElementById(id)
}


function semverCompare(a, b) 
{
  a = a.replace('v','')
  b = b.replace('v','')
  if (a.startsWith(b + '-')) return -1
  if (b.startsWith(a + '-')) return  1
  return a.localeCompare(b, undefined, { numeric: true, sensitivity: 'case', caseFirst: 'upper' })
}

function getfileName(asset)
{
  return asset.browser_download_url.split('/').pop()
}

async function getLatestGithubRelease(currentVersion)
{
  const jsonData = await (await fetch('https://api.github.com/repos/speeduino/AirBear/releases/latest')).json();
  const latestVersion = jsonData.tag_name;
  getElementByID('latest_release_txt').innerHTML = latestVersion
  if(semverCompare(latestVersion, currentVersion) == 1) //Value of 1 means a > b
  {
    getElementByID('update_btn').disabled = false

    for(const asset of jsonData.assets)
    {
      if(asset.name.includes('littlefs'))
      {
        const newData_url = 'http://speeduino.com/fw/AirBear/' + latestVersion + '/' + getfileName(asset)
        getElementByID('newData_url').value = newData_url
        console.log("Data file: " + newData_url)
      }
      else
      {
        const newFW_url = 'http://speeduino.com/fw/AirBear/' + latestVersion + '/' + getfileName(asset)
        getElementByID('newFW_url').value = newFW_url
        console.log("FW file: " + newFW_url)
      }
    }
  }
}

async function scanWifi()
{
  const s = getElementByID('ssid')
  const jsonData = await (await fetch('/wifi')).json()
  for(const network of jsonData.networks)
  {
    const opt = document.createElement('option');
    opt.value = network.ssid;
    opt.text = network.ssid;
    s.add(opt)
  }
}

function toggleData()
{
  const dataField = getElementByID('newData_url')
  dataField.disabled = !dataField.disabled
}

function setInnerHTML(id, val)
{
  id.innerHTML = val
}

function updateProgress()
{
  setTimeout(async () => {
    let jsonData
    try {
    const response = await fetch('/updateStatus')
    jsonData = await response.json();
    }
    catch (error)
    { 
      console.log(error)
      updateProgress() 
    }

    if(jsonData)
    {
      setInnerHTML(getElementByID('updateStatus'), jsonData.updateStatus)
      setInnerHTML(getElementByID('updateComplete'), jsonData.updateProgress)
      setInnerHTML(getElementByID('updateSize'), jsonData.updateSize)
      const percentComplete = Math.floor((jsonData.updateProgress / jsonData.updateSize) * 100)
      setInnerHTML(getElementByID('updatePercent'), percentComplete)
      console.log(percentComplete)
      if(percentComplete >= 98) { window.location.href = '/' } 
      else { updateProgress() }
    }
  }, 1500);
}
