
### order by
排序，支持多列排序，默认升序，需要降序输入desc

### where
*  当和order by一起使用时，order by位于where之后
*  检查空值 where is null
* in 例如： select * from stu where age in(1,3) 类似 age== 1 or age == 3
### 通配符
注意大部分通配符不能使用到索引，123% 能使用部分索引，%312不能使用索引
* % 匹配任意个字符，可位于前或者后 例如%ad%
* _ 匹配1个字符
### 正则表达式
regexp
select * from stu where addr regexp '[123]'

* 匹配特殊字符需要转义 \\
* 定位符* 和 ^ 可以指定从开头或结尾开始匹配
* 可以利用select来检查正则表达式。 eg: select 'hello' regexp '[a-z]'
### 计算字段
select统计出的字段并非在表中字段，而是由表中字段计算而得出
* concat 字段拼接
   eg: select CONCAT(name,'_',age) as col from stu
* trim ltrim rtrim 去除空格
*  算术运算 
* 日期处理函数Date  Year month 等
* 数值处理 abs

### 聚集函数
* avg sum min max count  用distinct修饰时，可以包含不同的值
    select * from
    
### 分组
group by, 在where之后，order by之前 

* having 过滤分组， 类似于where的用法，having用来设定某些条件来过滤分组，where之前能用语法这里也能用 . where 在是数据分组前进行过滤，having在数据分组后进行过滤
    eg: SELECT cityid, count(*) as cityPerSonNum  from stu GROUP BY cityid HAVING cityid > 2
    

### 子查询和联合表
多表直接 即可用 子查询（select结果 作为 其他select的 来源） ，也可以进行多表连接（inner join/left join/right join/full join等）.
inner join:

* 外键（foreign key）: 外键为某个表中的一列，它包含另一个表的主键值，定义了两个表之间的关系。
* 内连接  inner join
        select * from s1 inner join s2 where s1.id == s2.id

