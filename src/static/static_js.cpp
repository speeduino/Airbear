#include "static_js.h"

String staticJS_updates()
{
    String js = "";
    js += "<script>";
    js += "function i(t){return document.getElementById(t)}function r(t,e){return t=t.replace('v',''),e=e.replace('v',''),t.startsWith(e+'-')?-1:e.startsWith(t+'-')?1:t.localeCompare(e,void 0,{numeric:!0,sensitivity:'case',caseFirst:'upper'})}function o(t){return t.browser_download_url.split('/').pop()}async function getLatestGithubRelease(t){var e,a=await(await fetch('https://api.github.com/repos/speeduino/AirBear/releases/latest')).json(),n=a.tag_name;if(1==r(i('latest_release_txt').innerHTML=n,t)){i('update_btn').disabled=!1;for(const s of a.assets)s.name.includes('littlefs')?(e='http://speeduino.com/fw/AirBear/'+n+'/'+o(s),i('newData_url').value=e):(e='http://speeduino.com/fw/AirBear/'+n+'/'+o(s),i('newFW_url').value=e)}}async function scanWifi(){var t=i('ssid');for(const a of(await(await fetch('/wifi')).json()).networks){var e=document.createElement('option');e.value=a.ssid,e.text=a.ssid,t.add(e)}}function toggleData(){var t=i('newData_url');t.disabled=!t.disabled}function a(t,e){t.innerHTML=e}function updateProgress(){setTimeout(async()=>{let t;try{var e=await fetch('/updateStatus');t=await e.json()}catch(t){updateProgress()}t&&(a(i('updateStatus'),t.updateStatus),a(i('updateComplete'),t.updateProgress),a(i('updateSize'),t.updateSize),e=Math.floor(t.updateProgress/t.updateSize*100),a(i('updatePercent'),e),98<=e?window.location.href='/':updateProgress())},1500)}";
    js += "</script>";

    return js;
}