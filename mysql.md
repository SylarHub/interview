# mysql相关

## 1:mysql安装和连接工具
[mysql安装图解](https://blog.csdn.net/m0_49284219/article/details/121972531)
[连接工具navicat](https://www.downkuai.com/soft/161548.html)

## 2. sql相关知识点
### 2.1 DDL(data defined languge)
* show
* show database
* use 数据库名称
* create database  if not exists 数据库名;
* drop database 数据库名
* drop table if exists 表名 : 速度最快，删除表所有结构和数据
* truncate table 表名: 删除所有数据，并重建索引 
* alter  table  指定表名 add 新列名 列类型(类型长度); 
* alter table  指定表名  modify  指定列名(字段)  新的类型(新的长度)
* alter  table 指定表名  change  旧的列名  新的列名  新类型(类型长度)
* alter  table  指定表名  drop  指定列名

### 2.2 DML(data manipulate languge)
* insert into 指定表(指定列) values(指定值)
* update 表名 set 字段1=值 字段2=值
* delete from 表名 where 条件指定词

### 2.3 DQL (data query languge)
核心，sql查询语句

关键结束点
* distinct
    去除重复的值
* limit
    limit(m,n) 从第m行开始，筛选出n行 ，也支持 limit M offset N
    小提示点
* sql语句不区分大小写，常常表名、字段小写，关键字大写， 空格会被忽略.
* sql查询
#### order by
排序，支持多列排序，默认升序，需要降序输入desc

#### where
*  当和order by一起使用时，order by位于where之后
*  检查空值 where is null
* in 例如： select * from stu where age in(1,3) 类似 age== 1 or age == 3
#### 通配符
注意大部分通配符不能使用到索引，123% 能使用部分索引，%312不能使用索引
* % 匹配任意个字符，可位于前或者后 例如%ad%
* _ 匹配1个字符
#### 正则表达式
regexp
select * from stu where addr regexp '[123]'

* 匹配特殊字符需要转义 \\
* 定位符* 和 ^ 可以指定从开头或结尾开始匹配
* 可以利用select来检查正则表达式。 eg: select 'hello' regexp '[a-z]'
#### 计算字段
select统计出的字段并非在表中字段，而是由表中字段计算而得出
* concat 字段拼接
   eg: select CONCAT(name,'_',age) as col from stu
* trim ltrim rtrim 去除空格
*  算术运算 
* 日期处理函数Date  Year month 等
* 数值处理 abs

#### 聚集函数
* avg sum min max count  用distinct修饰时，可以包含不同的值
    select * from
    
#### 分组
group by, 在where之后，order by之前 

* having 过滤分组， 类似于where的用法，having用来设定某些条件来过滤分组，where之前能用语法这里也能用 . where 在是数据分组前进行过滤，having在数据分组后进行过滤
    eg: SELECT cityid, count(*) as cityPerSonNum  from stu GROUP BY cityid HAVING cityid > 2
    

#### 子查询和联合表
多表直接 即可用 子查询（select结果 作为 其他select的 来源） ，也可以进行多表连接（inner join/left join/right join/full join等）.
inner join:

* 外键（foreign key）: 外键为某个表中的一列，它包含另一个表的主键值，定义了两个表之间的关系。
* 内连接  inner join
        select * from s1 inner join s2 where s1.id == s2.id
这里有张图很详细解释了各种连接
![](https://cdn.jsdelivr.net/gh/SylarHub/imageHost@master/img202306181856994.png)

####     全文索引
Match 和 Against
select text from stu where Match(text) against("sylar") : 搜寻text字段中包含sylar
需要在创建表或者后续修改表时，设置text字段： fulltext(text)

### 2.4视图
视图是虚拟的表，只包含动态检索出的数据，对sql查询的一种封装. 主要作用有
* 重用sql，
*  保护数据 ，保护数据的格式和表示

视图是用来查看存储在别的地方的数据的一种方式，源表数据修改后，对视图的查看也会更新。另外视图之间还可以嵌套
使用方式：
create view 
    CREATE VIEW testView AS
    SELECT * from info where id % 2 = 0;
show create view 视图名

### 2.5存储过程
封装一系列sql操作. 可以控制权限，提高性能。

* 创建：这是一个无参数的存储过程
CREATE  PROCEDURE testfun()
BEGIN
	SELECT *FROM stu where id >= 5;
END
* 调用
call testfun();

上面就是简单的，如果是带参数的，可以指定IN OUT INOUT
CREATE DEFINER=`root`@`localhost` PROCEDURE `getName`(IN number INT,OUT addr VARCHAR(20))
BEGIN
	SELECT address from info where id = number INTO addr;
END

使用时直接调用
在进阶使用中，可以在存储过程里根据执行结果做一系列操作，把存储过程当做一个函数接口就行.
### 2.6触发器
在表改动时自动调用处理，这就是触发器。需要提供如下信息：在何种操作（insert 、 update、delete）下触发，何时触发(之前或之后)
### 2.7事务
保证一系列sql执行的完整性。
start transaction   (开始事务)
rollback (回退)
commit (提交)
savepoint (保留点)： 回退时可以回退到保留点，savepoint a;  rollback to a
在表中数据 之前或之后，调用该触发器。

## 3.索引
精彩文摘：[三友的java日记-索引的知识点](https://mp.weixin.qq.com/s/yibYJ9CQwiiVygu6vCZTFQ)
### 3.1 目的
通过索引的建立可以加快查询的速度

### 3.2索引建立
索引建立有几种方式，在建表时创建，alter表格,create index.
参考[索引建立](https://blog.csdn.net/weixin_46972127/article/details/127081023)

### 3.3 查看索引
sql语句前加上explain，可以模拟优化器如何对sql进行操作，返回使用了何种索引等. 详细可参考[explain用法](https://mp.weixin.qq.com/s/PrXZrZkOZDqp1BZCHviTsg).
重点关注type列，使用是哪种索引: system > const > eq_ref > ref > ref_or_null > index_merge > unique_subquery > index_subquery > range > index > ALL

### 3.4 结构
索引中用的最多的数据结构就是b+树，innodb。
一般来说，mysql数据库的B+树一般三层就可以放下几千万条数据（）