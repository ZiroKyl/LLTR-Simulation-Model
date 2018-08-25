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

	// http://highlightjs.readthedocs.io/en/latest/api.html#configure-options
	hljs.configure({
		languages: ["nohighlight"] //turn off auto language detection ( "nohighlight" - https://highlightjs.org/usage/ -> https://github.com/highlightjs/highlight.js/blob/9.12.0/src/highlight.js#L560 )
	});
	hljs.initHighlighting();

}, false);
