function LoadHeader(section,BigTitle,MidlleTitle,SmallTitle)
{
	text="<table class=tblBigWB width=100%><tr><td width=65%>"+
   		BigTitle+"</td><td style='font-size:xx-small'><p align=right>"+
   		MidlleTitle+"</p></td></tr><tr><td style='font-size:xx-small'>"+
   		SmallTitle+"</td></tr></table>";
 	section.outerHTML=text;
}

function LoadMenu(section)
{
	text="<table class=menu width=100%>";
	title=Array("Сборище плагинов","OpenConf","Разработчикам","Обсудить","Книга жалоб и предложений","@");
	href=Array("/plugins","/oc.html","/develop","aaa","aaa","mailto:openconf@narod.ru");
	for(i=0;i<6;i++)
		text+="<td align=center><a class=white href="+href[i]+">"+title[i]+"</a> </td>";
	text+="</tr></table>";
 	section.outerHTML=text;
}
