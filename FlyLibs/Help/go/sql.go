
	"strconv"
	"strings"
	"time"

// 将结果集转换成map
func getMapFromRows(rows *sql.Rows) ([]map[string]interface{}, error) {
	var ret []map[string]interface{}
	cols, _ := rows.Columns()
	colTypes, err := rows.ColumnTypes()

	if err != nil {
		fmt.Printf("read coltype error:%v\n", err.Error())
	}
	for rows.Next() {
		columns := make([]interface{}, len(cols))
		columnPointers := make([]interface{}, len(cols))
		for i, _ := range columns {
			columnPointers[i] = &columns[i]
		}

		if err := rows.Scan(columnPointers...); err != nil {
			return ret, err
		}
		m := make(map[string]interface{})
		for i, colName := range cols {
			colType := colTypes[i]
			value := *columnPointers[i].(*interface{})

			if value == nil {
				m[colName] = nil
				continue
			}

			dbTypeName := colType.DatabaseTypeName()

			if dbTypeName == "MONEY" {
				arrValue := value.([]uint8)
				floatValue, _ := strconv.ParseFloat(string(arrValue), 64)
				m[colName] = floatValue
			} else if dbTypeName == "DATE" {
				timeValue := value.(time.Time)
				m[colName] = timeValue.Format("2006-01-02")
			} else if dbTypeName == "DATETIME" {
				timeValue := value.(time.Time)
				m[colName] = timeValue.Format("2006-01-02 15:04:05")
			} else {
				m[colName] = value
			}
		}
		ret = append(ret, m)
	}
	return ret, nil
}