
/*
go  ajax: ---https://studygolang.com/articles/2671

http.Handle("/js/", http.FileServer(http.Dir("web")))   服务器目录对应  web/js      html  对应   <script type="text/javascript" src="js/index.js"></script> 
http.Handle("/css/", http.FileServer(http.Dir("web")))   服务器目录对应  web/css      html  对应  <link type="text/css" rel="stylesheet" href="css/index.css" /> 


获取body
  body, _ := ioutil.ReadAll(r.Body) 
  log.Printf("body: %v", string(body))

*/