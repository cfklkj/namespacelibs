/*
初始化
function main() {
    onLoginInit();
    var oBtn = document.getElementById('test');
    oBtn.onclick = GetSelectType;
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
*/