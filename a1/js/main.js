//////////////////////////
//
"use strict";

document.addEventListener("DOMContentLoaded", function(){
	(function(){
		var code = document.querySelectorAll(":not(pre) > code"); //для Хабра: ".post__text :not(pre) > code"

		var unQuote = function(el){
			var aN = null, a = "",
				bN = null, b = "";

			if((aN = el.previousSibling) && (bN = el.nextSibling) && 
				aN.nodeType == 3         &&  bN.nodeType == 3     ){

				a = aN.data; a = a.charCodeAt(a.length-1);
				b = bN.data; b = b.charCodeAt(0);

				// a != " " && ( a+1 == b && (a == "“"  || a == "‘" )  ||  (a == "«" && b == "»")  ||  a == b && (a == "'" || a == "`" || a == '"') )
				if(a != 32  && ( a+1 == b && (a == 8220 || a == 8216)  ||  (a == 171 && b == 187)  ||  a == b && (a == 39  || a == 96  || a == 34 ) )){
					aN.data = aN.data.slice(0,-1);
					bN.data = bN.data.slice(1);

					return true;
				}
			}

			return false;
		};


		var aN = null, bN = null, pElTagName = "";

		for(var i=code.length;i--;){
			// “<code>...</code>”  ->  <code>...</code>
			if(!unQuote(code[i])                                                                  && 
				( code[i].previousElementSibling == null && code[i].nextElementSibling == null && 
				  (pElTagName = code[i].parentElement.tagName)                                 && 
				  (pElTagName == "A" || pElTagName == "STRONG")                                 ) &&
				( (!(aN = code[i].previousSibling) || aN.data.trim().length == 0) &&
				  (!(bN = code[i].nextSibling)     || bN.data.trim().length == 0)  )              ){
				// “<a><code>...</code></a>”     ->  <a><code>...</code></a>
				// “<a> <code>...</code> </a>”   ->  <a> <code>...</code> </a>
				// “<a><code>...</code>...</a>”  -X
				unQuote(code[i].parentElement);
			}
		}
	})();

	////////////////////////////////////////////////////////////////////////////////////////////

	if(!window.HTMLDetailsElement){
		var summary = document.querySelectorAll("details > summary");

		var switcher = function(){
			var details = this.parentElement;

			if(details.hasAttribute("open")) details.removeAttribute("open");
			else                             details.setAttribute("open", "");
		};

		for(var i=summary.length;i--;) summary[i].addEventListener("click", switcher, false);

		summary = switcher = null; //:-)
	}

	////////////////////////////////////////////////////////////////////////////////////////////

	(function(){
		var _run_ = document.querySelectorAll("#do_add-target-to-simulation-manual-a")[0];
		var _localStorageKey = "lltr.a1.sim_man_target";
		
		// localStorage benchmark: removeItem() vs setItem()
		// var a=0,i=0,s="very.looooong_and-unique key";
		//
		// a=Date.now();for(i=8888;i--;){ localStorage.setItem(s,"");  localStorage.removeItem(s);  };Date.now()-a;
		// a=Date.now();for(i=8888;i--;){ localStorage.setItem(s,"1"); localStorage.setItem(s,"0"); };Date.now()-a;
		
		var saveOn  = function(){ localStorage.setItem(_localStorageKey, "1") };
		var saveOff = function(){ localStorage.setItem(_localStorageKey, "0") };

		var addTarget = function(){
			var m = document.querySelectorAll('a[href^="https://omnetpp.org/doc/omnetpp/manual/"]');
			var target = "omnetpp-sm";
			var color  = "#993200";
			var sw     = false;

			if(m.length == 0) return;

			if(sw = m[0].target==target){ //switch
				color = target = "";
				saveOff();
			}

			for(var i=m.length;i--;){
				m[i].target      = target;
				m[i].style.color = color;
			}
			
			if(!sw){
				saveOn();
			}
		};

		_run_.addEventListener("click", addTarget, false);
		
		try{
			if(localStorage.getItem(_localStorageKey) == "1") addTarget();
		}catch(e){
			// https://stackoverflow.com/questions/30481516/iframe-in-chrome-error-failted-to-read-localstorage-from-window-access-den/44288957#44288957
			// https://forum.vivaldi.net/topic/26061/local-storage
			// https://michalzalecki.com/why-using-localStorage-directly-is-a-bad-idea/
			// http://www.chromium.org/for-testers/bug-reporting-guidelines/uncaught-securityerror-failed-to-read-the-localstorage-property-from-window-access-is-denied-for-this-document
			saveOn = saveOff = function(){};
			
			console.error(e, "\n-> Click to blocked cookies icon in the address bar");
			
			addTarget();
		}
		
		_run_ = addTarget = null; //:-)
	})();

	////////////////////////////////////////////////////////////////////////////////////////////

	// http://highlightjs.readthedocs.io/en/latest/api.html#configure-options
	hljs.configure({
		languages: ["nohighlight"] //turn off auto language detection ( "nohighlight" - https://highlightjs.org/usage/ -> https://github.com/highlightjs/highlight.js/blob/9.12.0/src/highlight.js#L560 )
	});
	hljs.initHighlighting();

}, false);

window.addEventListener("load", function(){
	if(this.location.hash == "#i-come-from-habr"){
		this.location.replace(this.location.pathname + this.location.search + "#i-came-from-habr");
	}
}, false);
