
/*
go  ajax: ---https://studygolang.com/articles/2671

http.Handle("/js/", http.FileServer(http.Dir("web")))   ������Ŀ¼��Ӧ  web/js      html  ��Ӧ   <script type="text/javascript" src="js/index.js"></script> 
http.Handle("/css/", http.FileServer(http.Dir("web")))   ������Ŀ¼��Ӧ  web/css      html  ��Ӧ  <link type="text/css" rel="stylesheet" href="css/index.css" /> 


��ȡbody
  body, _ := ioutil.ReadAll(r.Body) 
  log.Printf("body: %v", string(body))

*/