#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <regex>

class SimpleJSON {
private:
    struct JSONData {
        std::unordered_map<std::string, std::string> keyValuePairs;
        std::unordered_map<std::string, std::vector<std::string>> arrays;
    };

    std::unordered_map<int, JSONData> jsonStorage;

    // 辅助函数：验证值是否符合指定类型
    bool isValueValidForType(const std::string& value, int type) {
        switch (type) {
        case 1: // 数字（整数或浮点数）
            return std::regex_match(value, std::regex(R"(^-?\d+(\.\d+)?$)"));
        case 2: // 字符串（在双引号中）
            return std::regex_match(value, std::regex(R"(^\".*\"$)"));
        case 3: // 逻辑值（true 或 false）
            return value == "true" || value == "false";
        case 4: // 数组（在中括号中）
            return std::regex_match(value, std::regex(R"(^\[(.*)\]$)"));
        case 5: // null
            return value == "null";
        default:
            return false; // 不支持的类型
        }
    }

public:
    // 1. 创建JSON对象
    int createJSON(int JSONID) {
        if (jsonStorage.find(JSONID) != jsonStorage.end()) {
            return 1; // JSONID 已存在
        }
        jsonStorage[JSONID] = JSONData();
        return 0; // 成功
    }

    // 2. 添加键值对到JSON对象中
    int addKeyValue(int JSONID, const std::string& key, const std::string& value, int type) {
        auto it = jsonStorage.find(JSONID);
        if (it == jsonStorage.end()) {
            return 2; // JSONID 不存在
        }
        if (it->second.keyValuePairs.find(key) != it->second.keyValuePairs.end()) {
            return 1; // 键名已存在
        }
        if (!isValueValidForType(value, type)) {
            return 3; // 键值不符合类型
        }
        it->second.keyValuePairs[key] = value;
        return 0; // 成功
    }

    // 3. 修改已存在的键值对
    int modifyKeyValue(int JSONID, const std::string& key, const std::string& newValue, int type) {
        auto it = jsonStorage.find(JSONID);
        if (it == jsonStorage.end()) {
            return 2; // JSONID 不存在
        }
        if (it->second.keyValuePairs.find(key) == it->second.keyValuePairs.end()) {
            return 1; // 键名不存在
        }
        if (!isValueValidForType(newValue, type)) {
            return 3; // 键值不符合类型
        }
        it->second.keyValuePairs[key] = newValue;
        return 0; // 成功
    }

    // 4. 删除键值对
    int deleteKey(int JSONID, const std::string& key) {
        auto it = jsonStorage.find(JSONID);
        if (it == jsonStorage.end()) {
            return 2; // JSONID 不存在
        }
        if (it->second.keyValuePairs.erase(key) == 0) {
            return 1; // 键名不存在
        }
        return 0; // 成功
    }

    // 5. 删除整个JSON对象
    int deleteJSON(int JSONID) {
        if (jsonStorage.erase(JSONID) == 0) {
            return 1; // JSONID 不存在
        }
        return 0; // 成功
    }

    // 6. 获取键的值
    std::string getValue(int JSONID, const std::string& key) {
        auto it = jsonStorage.find(JSONID);
        if (it == jsonStorage.end()) {
            return ""; // JSONID 不存在
        }
        auto keyIt = it->second.keyValuePairs.find(key);
        if (keyIt == it->second.keyValuePairs.end()) {
            return ""; // 键名不存在
        }
        return keyIt->second; // 返回键值
    }

    // 7. 用文本型JSON数据替换整个JSON对象的数据
    int setJSONFromText(int JSONID, const std::unordered_map<std::string, std::string>& jsonData) {
        auto it = jsonStorage.find(JSONID);
        if (it == jsonStorage.end()) {
            return 1; // JSONID 不存在
        }
        it->second.keyValuePairs = jsonData; // 覆盖掉原有的数据
        return 0; // 成功
    }

    // 8. 添加数组到JSON对象
    int addArray(int JSONID, const std::string& key, const std::vector<std::string>& array) {
        auto it = jsonStorage.find(JSONID);
        if (it == jsonStorage.end()) {
            return 2; // JSONID 不存在
        }
        if (it->second.arrays.find(key) != it->second.arrays.end()) {
            return 1; // 数组键名已存在
        }
        it->second.arrays[key] = array;
        return 0; // 成功
    }

    // 9. 删除整个数组
    int deleteArray(int JSONID, const std::string& key) {
        auto it = jsonStorage.find(JSONID);
        if (it == jsonStorage.end()) {
            return 2; // JSONID 不存在
        }
        if (it->second.arrays.erase(key) == 0) {
            return 1; // 数组键名不存在
        }
        return 0; // 成功
    }

    // 10. 输出指定JSONID的整个JSON数据
    std::string getJSONData(int JSONID) {
        auto it = jsonStorage.find(JSONID);
        if (it == jsonStorage.end()) {
            return ""; // JSONID 不存在
        }

        std::ostringstream oss;
        oss << "{\n";

        // 输出键值对
        for (const auto& pair : it->second.keyValuePairs) {
            oss << "  \"" << pair.first << "\": " << pair.second << ",\n";
        }

        // 输出数组数据
        for (const auto& arrayPair : it->second.arrays) {
            oss << "  \"" << arrayPair.first << "\": [";
            for (size_t i = 0; i < arrayPair.second.size(); ++i) {
                oss << "\"" << arrayPair.second[i] << "\"";
                if (i < arrayPair.second.size() - 1) {
                    oss << ", ";
                }
            }
            oss << "],\n";
        }

        oss << "}";
        return oss.str(); // 返回整个JSON数据的字符串表示
    }
};

int main() {
    SimpleJSON jsonManager;
    int result;

    // 示例操作
    result = jsonManager.createJSON(1);
    std::cout << "创建JSON结果: " << result << std::endl;

    result = jsonManager.addKeyValue(1, "age", "25", 1);
    std::cout << "添加数字键值对结果: " << result << std::endl;

    result = jsonManager.addKeyValue(1, "name", "\"John\"", 2);
    std::cout << "添加字符串键值对结果: " << result << std::endl;

    result = jsonManager.addKeyValue(1, "isStudent", "true", 3);
    std::cout << "添加逻辑值键值对结果: " << result << std::endl;

    result = jsonManager.addKeyValue(1, "data", "[1, 2, 3]", 4);
    std::cout << "添加数组键值对结果: " << result << std::endl;

    result = jsonManager.addKeyValue(1, "nothing", "null", 5);
    std::cout << "添加null键值对结果: " << result << std::endl;

    result = jsonManager.addKeyValue(1, "invalid", "not_a_number", 1);
    std::cout << "添加不符合类型的键值对结果: " << result << std::endl;

    std::string jsonData = jsonManager.getJSONData(1);
    std::cout << "获取整个JSON数据:\n" << jsonData << std::endl;

    return 0;
}
