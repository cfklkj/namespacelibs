
/*
get
go get -u -v github.com/nsf/gocode


odbc
https://www.cnblogs.com/zsy/p/5226740.html
https://studygolang.com/articles/8314
https://blog.csdn.net/kenkao/article/details/47857795

go
git merge --abort


json

type S2CDirInfo struct{
    HomePath string  `json:"homePath"`  //��Ҫ���(`json:"homePath"` )����תjson
    Chils []DirInfo `json:"chis"`
}


	ret, err := json.Marshal(&s2cInfo)
	if err != nil {
		//c.String(500, "encode json")
		return err.Error()
    }
    fmt.Print(string(ret))
    return string(ret); 
*/