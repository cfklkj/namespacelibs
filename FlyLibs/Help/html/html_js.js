/*
��ʼ��
function main() {
    onLoginInit();
    var oBtn = document.getElementById('test');
    oBtn.onclick = GetSelectType;
    oBtn.innerHTML = ""; //select ���
    oBtn.onchange = onok; //select �󶨺���
}
��ȡselect��Ϣ 
    var project = document.getElementById('proInfo'); 
    var indexA = project.selectedIndex; 
    var serverName = project.options[indexA].id; 
����select
    var ActInfo = document.getElementById('ActInfo');
    ActInfo.options[j]=new Option(jsonInfo[j].displayName,value); 
    ActInfo.options[j].id = jsonInfo[j].name;
    ActInfo.options[j].value = jsonInfo[j].type;
����get post 
    var xhr = new XMLHttpRequest();
    xhr.open('post', serverUrl + '/services/' + serverName + '/query/action/' + displayName, true);
    xhr.setRequestHeader("token", MyToken); 
    xhr.send(); //xhr.send(JSON.stringify(userInfo));

    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4) { // ��ȡ���
            if (xhr.status == 200) { 
                var jsonInfo = JSON.parse(xhr.responseText)
                }
           }
    }

����table
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

 ɾ��table
  for (i=1;i< table.rows.length;)
        {
             table.deleteRow(i);  
        }  
 
 ��ȡ�Զ�������
 table.getAttribute("MyValues")

 ����
  var project = document.getElementById('selectMenu');
   for(i=0;i<program.length;i++)   
  {
    var id = program[i].getAttribute("value");
    var input = program[i].getElementsByTagName("input");
    if(input[0].value.length > 0)
   }

  
ɾ���ڵ�
 chileName.parentNode.removeChild(chileName);

 checkBox
 if(project.checked)
    alert("ѡ��")

���ù�����
div css   
height: 60%;
overflow-y: auto;

div�Ŀ��Ҫ����table�Ŀ��


html ���document.body.clientWidth



string--------------
string to array   -->  A = B.split(',')
�������ַ���  A.indexOf(B) > 0


json----------------
var userInfo = {"userName":vA, "password":vB};

JSON.parse(jsonstr); //���Խ�json�ַ���ת����json���� 
JSON.stringify(jsonobj); //���Խ�json����ת����json�Է��� 

����
var json = [{dd:'SB',AA:'����',re1:123},{cccc:'dd',lk:'1qw'}];
for(var i=0,l=json.length;i<l;i++){
    for(var key in json[i]){
        alert(key+':'+json[i][key]);
    }
}

css-------------
border-left:1px solid #000 ���ö������1pxʵ�ߺ�ɫ�߿�

html------
<input type="file" id="file_input" multiple="multiple" />  ѡ���ļ�
<input type="file" id="file_input" webkitdirectory directory /> ѡ��Ŀ¼
*/


