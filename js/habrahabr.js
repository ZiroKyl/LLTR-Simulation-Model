//////////////////////////
//
"use strict";


document.addEventListener("DOMContentLoaded", function(){
	var el = document.querySelectorAll("anchor");
	for(var i=el.length;i--;){
		el[i].outerHTML = '<a name="' + el[i].textContent + '"></a>';
	}
	
	el = document.querySelectorAll("spoiler");
	for(i=el.length;i--;){
		el[i].outerHTML = '<details>\n<summary>' + el[i].title + '</summary>' + el[i].innerHTML + '</details>';
	}
	
	/*el = document.querySelectorAll("source");
	for(i=el.length;i--;){
		el[i].outerHTML = '<pre><code class="brush: ' + el[i].lang + ';">' + el[i].innerHTML + '</code></pre>';
	}*/
	
	el = document.querySelectorAll("cut")[0];
	el.outerHTML = '<p><br />\n<strong>&lt;!--more--&gt;</strong>\n</p>' + el.innerHTML;
}, true);
