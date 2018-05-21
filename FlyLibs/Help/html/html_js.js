/*
初始化
function main() {
    onLoginInit();
    var oBtn = document.getElementById('test');
    oBtn.onclick = GetSelectType;
    oBtn.innerHTML = ""; //select 清空
    oBtn.onchange = onok; //select 绑定函数
}
获取select信息 
    var project = document.getElementById('proInfo'); 
    var indexA = project.selectedIndex; 
    var serverName = project.options[indexA].id; 
插入select
    var ActInfo = document.getElementById('ActInfo');
    ActInfo.options[j]=new Option(jsonInfo[j].displayName,value); 
    ActInfo.options[j].id = jsonInfo[j].name;
    ActInfo.options[j].value = jsonInfo[j].type;
发送get post 
    var xhr = new XMLHttpRequest();
    xhr.open('post', serverUrl + '/services/' + serverName + '/query/action/' + displayName, true);
    xhr.setRequestHeader("token", MyToken); 
    xhr.send(); //xhr.send(JSON.stringify(userInfo));

    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4) { // 读取完成
            if (xhr.status == 200) { 
                var jsonInfo = JSON.parse(xhr.responseText)
                }
           }
    }

插入table
    var table = document.getElementById('details'); 
    var jsonInfo = JSON.parse(xhr.responseText)
    var value =0;
    var tr,td;  
    var headTd = table.getElementsByTagName("td")
    for(var info=0; jsonInfo[info];info++){  //json
        tr = table.insertRow(table.rows.length);  //row
        for (var i=0;i<headTd.length;i++) {  //table
            for(var key in jsonInfo[info])
            {                        
                if(key == headTd[i].id)  //col
                {                            
                    td = tr.insertCell(tr.cells.length);
                    td.innerHTML = jsonInfo[info][key];  
                    td.align = "center";
                    break;
                }
            }                        
        } 
    }

 删除table
  for (i=1;i< table.rows.length;)
        {
             table.deleteRow(i);  
        }  
 
 获取自定义属性
 table.getAttribute("MyValues")

 遍历
  var project = document.getElementById('selectMenu');
   for(i=0;i<program.length;i++)   
  {
    var id = program[i].getAttribute("value");
    var input = program[i].getElementsByTagName("input");
    if(input[0].value.length > 0)
   }

  
删除节点
 chileName.parentNode.removeChild(chileName);

 checkBox
 if(project.checked)
    alert("选中")

设置滚动条
div css   
height: 60%;
overflow-y: auto;

div的宽度要大于table的宽度


html 宽度document.body.clientWidth



string--------------
string to array   -->  A = B.split(',')
查找子字符串  A.indexOf(B) > 0


json----------------
var userInfo = {"userName":vA, "password":vB};

JSON.parse(jsonstr); //可以将json字符串转换成json对象 
JSON.stringify(jsonobj); //可以将json对象转换成json对符串 

遍历
var json = [{dd:'SB',AA:'东东',re1:123},{cccc:'dd',lk:'1qw'}];
for(var i=0,l=json.length;i<l;i++){
    for(var key in json[i]){
        alert(key+':'+json[i][key]);
    }
}

css-------------
border-left:1px solid #000 设置对象左边1px实线黑色边框

html------
<input type="file" id="file_input" multiple="multiple" />  选择文件
<input type="file" id="file_input" webkitdirectory directory /> 选择目录
*/


