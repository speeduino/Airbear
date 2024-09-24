#include "static_js.h"

String staticJS_updates()
{
    String js = "";
    js += "<script>";
    js += "function i(e){return document.getElementById(e)}function r(e,t){return e=e.replace('v',''),t=t.replace('v',''),e.startsWith(t+'-')?-1:t.startsWith(e+'-')?1:e.localeCompare(t,void 0,{numeric:!0,sensitivity:'case',caseFirst:'upper'})}function o(e){return e.browser_download_url.split('/').pop()}async function getLatestGithubRelease(e){var t,a=await(await fetch('https://api.github.com/repos/speeduino/AirBear/releases/latest')).json(),n=a.tag_name;if(1==r(i('latest_release_txt').innerHTML=n,e)){i('update_btn').disabled=!1;for(const s of a.assets)s.name.includes('littlefs')?(t='http://speeduino.com/fw/AirBear/'+n+'/'+o(s),i('newData_url').value=t):(t='http://speeduino.com/fw/AirBear/'+n+'/'+o(s),i('newFW_url').value=t)}}async function scanWifi(){var e=i('ssid');for(const a of(await(await fetch('/wifi')).json()).networks){var t=document.createElement('option');t.value=a.ssid,t.text=a.ssid,e.add(t)}}function toggleData(){var e=i('newData_url');e.disabled=!e.disabled}function a(e,t){e.innerHTML=t}function updateProgress(){setTimeout(async()=>{let e;try{var t=await fetch('/updateStatus');e=await t.json()}catch(e){updateProgress()}e&&(a(i('updateStatus'),e.updateStatus),a(i('updateComplete'),e.updateProgress),a(i('updateSize'),e.updateSize),t=Math.floor(e.updateProgress/e.updateSize*100),a(i('updatePercent'),t),98<=t?setTimeout(()=>{var e=window.location;e.href='/',e=e.href},7000):updateProgress())},1100)}";
    js += "</script>";

    return js;
}