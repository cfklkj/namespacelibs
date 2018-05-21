

    "io/ioutil" 
    "encoding/json" 

	data, err := ioutil.ReadFile(filename)

	json.Unmarshal(data, &JsonParse)  注意传的是地址