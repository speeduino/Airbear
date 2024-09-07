function eByID(id)
{
  return document.getElementById(id)
}

function semverCompare(a, b) 
{
  a = a.replace("v","")
  b = b.replace("v","")
  if (a.startsWith(b + "-")) return -1
  if (b.startsWith(a + "-")) return  1
  return a.localeCompare(b, undefined, { numeric: true, sensitivity: "case", caseFirst: "upper" })
}

function getfName(asset)
{
  return asset.browser_download_url.split('/').pop()
}

async function getLatestGithubRelease(currentVersion)
{
  const jsonData = await (await fetch('https://api.github.com/repos/speeduino/AirBear/releases/latest')).json();
  const latestVersion = jsonData.tag_name;
  eByID("latest_release_txt").innerHTML = latestVersion
  if(semverCompare(latestVersion, currentVersion) == 1) //Value of 1 means a > b
  {
    eByID("update_btn").disabled = false

    for(const asset of jsonData.assets)
    {
      if(asset.name.includes("littlefs"))
      {
        const newData_url = "http://speeduino.com/fw/AirBear/" + latestVersion + "/" + getfName(asset)
        eByID("newData_url").value = newData_url
        console.log("Data file: " + newData_url)
      }
      else
      {
        const newFW_url = "http://speeduino.com/fw/AirBear/" + latestVersion + "/" + getfName(asset)
        eByID("newFW_url").value = newFW_url
        console.log("FW file: " + newFW_url)
      }
    }
  }
}

async function scanWifi()
{
  const s = eByID("ssid")
  const jsonData = await (await fetch('/wifi')).json()
  for(const network of jsonData.networks)
  {
    const opt = document.createElement("option");
    opt.value = network.ssid;
    opt.text = network.ssid;
    s.add(opt)
  }
}