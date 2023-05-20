# redis

## 数据结构
redis对外来说，提供了五种基本的数据类型。字符串(string)、列表(list)、哈希(hash)、集合(set)、有序集合(zset). 后续版本也提供了BitMap,HyperLogLog,GEO,Stream等。
重点参考来源
[小林code redis数据结构](https://xiaolincoding.com/redis/data_struct/data_struct.html#quicklist-%E7%BB%93%E6%9E%84%E8%AE%BE%E8%AE%A1)
[小林code redis数据结构及其应用场景](https://xiaolincoding.com/redis/data_struct/command.html#%E4%BB%8B%E7%BB%8D)

## string
键值对数据，可以用SDS实现
对比c语言的字符串，在以下方面增强了
1. 原先获取长度需要遍历字符串。现在新增一个变量用于获取数据长度
2. 原先字符串以'\0'结尾。现在为了处理二进制数据(如图片等)，根据数据长度字段数据何时截止
3. 原先可能会发生缓冲区溢出，需要用户进行保证不溢出。现在可以动态拓容。根据已分配的字符串