function initPage(pages){
	str = "<ul class='pagination pull-right'>";
	str += "<li class='first disabled'><a>first</a></li>";
	str += "<li class='pre disabled'><a>previous</a></li>";
	if (pages>0){
		str += "<li class='number disabled 1'><a>1</a></li>";
	}
	for ( var i = 2 ; i <= Math.min(pages,5) ; i++ ){
		str += "<li class='number "+i+"'><a>"+i+"</a></li>";
	}
	if (pages>1){
		str += "<li class='next'><a>next</a></li>";
	}
	else{
		str += "<li class='next disabled'><a>next</a></li>";
	}
	str += "</ul>";
	$("div[class*='page']").html(str);
//	listStatus(1);
}

$(function() {
	
	$(document).on("click","div.filter div.result li",function(){
		var str =$(this).text().trim();
		str+="<span class='caret'></span>";
		$("div.filter div.result button"). html(str);
	});
	
	$(document).on("click","div.filter div.lang li",function(){
		var str =$(this).text().trim();
		str+="<span class='caret'></span>";
		$("div.filter div.lang button"). html(str);
	});
	
	$(document).on("click","div.filter div.pid li",function(){
		var str =$(this).text().trim();
		str+="<span class='caret'></span>";
		$("div.filter div.pid button"). html(str);
	});
	
	$(document).on("click","div.showSourcesCode",function(){
		var id = this.className.substr(16);
		$.post("/OnlineJudge/status/show_run","runid="+id,function(data){
			var dat = JSON.parse(data);
			$("div[id='showCode-dialog'] textarea").val(dat.status_code);
			$("div[id='showCode-dialog']").modal('toggle');
		})
	});	
	
	$(document).on("click","div.showCEreason",function(){
		var id = this.className.substr(13);
		$.post("/OnlineJudge/status/show_ce_reason","runid="+id,function(data){
			$("div[id='showCEreason-dialog']").modal('toggle');
			$("div[id='showCEreason-dialog'] textarea").val(data);
		})
	});
	
	$("div[class*='btn'][id='show_user']").click(function(){
		location.href="/OnlineJudge/user/show_user/name/"+this.innerText;
	})
	
	$(document).on("click",".pagination li",function(){
		if ($(this).hasClass("disabled")){
			return false;
		}
		else{
			var cur = $(this).text();
			if (cur=="previous"||cur=="next"){
				var th = $(".pagination li[class*='number'][class*='disabled']");
				if (cur=="previous"){
					cur = parseInt(th[0].innerText)-1;
				}
				else{
					cur = parseInt(th[0].innerText)+1;
				}
			}
			if (cur=="first"){
				cur=1;
			}
			$(".pagination li").each(function(){
				$(this).removeClass("disabled");
			});
			var pages = get_totPages(0);
			str = "";
			if (cur==1){
				str += "<li class='first disabled'><a>first</a></li>";
				str += "<li class='pre disabled'><a>previous</a></li>";
			}
			else{
				str += "<li class='first'><a>first</a></li>";
				str += "<li class='pre'><a>previous</a></li>";
			}
			if (pages<=5){
				for ( var i = 1 ; i <= pages ; i++ ){
					if (i==cur){
						str += "<li class='number disabled'><a>"+i+"</a></li>";
					}
					else{
						str += "<li class='number'><a>"+i+"</a></li>";
					}
				}
			}
			else{
				var start = parseInt(cur)-2, end = parseInt(cur)+2;
				if (start<1){
					end += 1-start;
					start = 1;
				}
				else if (end>pages){
					start -= end-pages;
					end = pages;
				}
				for ( var i = start, j = 1 ; i <= end ; i++ ){
					if (i==cur){
						str += "<li class='number disabled'><a>"+i+"</a></li>";
					}
					else{
						str += "<li class='number'><a>"+i+"</a></li>";
					}
				}
			}
			if (cur==pages){
				str += "<li class='next disabled'><a>next</a></li>";
			}
			else{
				str += "<li class='next'><a>next</a></li>";
			}
			str += "</li>";
			$("ul[class*='pagination']").html(str);
			listPage(cur);
		}
	});
	
	$("#back").click(function() {
		window.history.back();
	});
});