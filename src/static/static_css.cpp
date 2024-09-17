
#include "static_css.h"

String staticCSS_config()
{
    String css = "";
    css += "<style type=\"text/css\">";
    css += "body,html{box-sizing:border-box;height:100%}.content>div,input.tabs{display:none}h1,label{text-align:center}body,h1{color:#fff}body{font-family:Arial;display:block;align-items:center;justify-content:center;padding:0;margin:0;background-color:#202020;background-image:linear-gradient(to right,#000,rgba(0,0,0,0) 30%,rgba(0,0,0,0) 70%,#000),linear-gradient(45deg,#000 25%,transparent 25%,transparent 75%,#000 75%,#000),linear-gradient(45deg,#000 25%,transparent 25%,transparent 75%,#000 75%,#000),linear-gradient(to bottom,#080808,#202020);background-size:100% 100%,10px 10px,10px 10px,10px 5px;background-position:0 0,0 0,5px 5px,0 0}small{opacity:.4}h1{text-shadow:2px 2px 2px rgb(0,0,0,.6);margin-bottom:0;font-size:1.5em;line-height:1em;font-weight:700;margin-top:.27em}.container{min-width:620px;max-width:700px;margin:30px auto 20px;width:50%}p{margin:0 0 20px;line-height:1.5;font-weight:300}.content{background:#fff;color:#373737;backface-visibility:hidden;overflow:hidden}.content>div{padding:20px 25px 5px}label{display:inline-block;padding:15px 25px;font-weight:600;border-bottom:1px solid transparent;transition:.3s ease-out .1s}label:hover{color:#fff;cursor:pointer}input:checked+label{background:#fff;color:#fd7034}#tab-1:checked~.content #content-1,#tab-2:checked~.content #content-2,#tab-3:checked~.content #content-3,#tab-4:checked~.content #content-4{display:block;animation-name:inUp;animation-timing-function:ease-in-out;animation-duration:.6s}@media screen and (max-width:400px){label{padding:15px 10px}}@keyframes inUp{0%{opacity:0;transform:translateY(100px)}90%{transform:translateY(-10px)}100%{opacity:1;transform:translateY(0)}}";
    css += "</style>";

    return css;
}