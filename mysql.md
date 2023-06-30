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

## 4.存储
### 4.1 buffer pool
#### 4.1.1结构
大部分引擎的mysql数据都是存储在硬盘之中，为了避免每次sql操作都需要进行一系列磁盘IO，Innodb 存储引擎 特此在内存中分配一片空间用作 缓冲区。
bufferpool中主要存储有数据页，索引页，锁结构，undo页、自适应hash、插入缓存等。 重点看数据页和缓存页。
#### 4.1.2 空间分配
 从内存分配上来看，bufferpool是一片连续的空间. 空间分布上通过控制块来各个页（分索引页。数据页等）
![空间](https://cdn.jsdelivr.net/gh/SylarHub/imageHost@master/img202306262255722.png)
#### 4.1.3 管理
* 空闲链表
    通过空闲链表来管理空页，需要时直接从空闲链表拉去
* 脏页
    为提高写性能，mysql在写入时如何命中缓存，直接将数据设置为已写，即脏页，在多种同步机制下回写入磁盘（从old区删除，定时存储，redo日志满，关闭时）
* 修改版LRU
   用于管理脏页和干净页。 通过设立young和old区来处理 预读失效问题（一次会读多页进缓存），通过一段时间内多次访问从old区进入young区 来解决buffer污染问题(多次读批量数据进来，将旧buffer给删除了).
## 5. 日志
主要摘选于[小林code之mysql日志](https://xiaolincoding.com/mysql/log/how_update.html#%E4%B8%BA%E4%BB%80%E4%B9%88%E9%9C%80%E8%A6%81-undo-log)
mysql日志主要有undo日志、redo日志、binlog日志三种。
### 5.1 undo日志
由innodb存储引擎支持。undo日志保证了事务的原子性，在事务执行过程中需要回滚，这时候如何实现呢？就是在执行事务时中的sql语句时，将需要回滚的信息记录进undo日志中(插入操作记录要插入的主键，删除操作记录当前记录，更新操作记录要更新的列的数据)。每次对记录进行操作时，记录的undo日志会形成版本链（trx_id代表操作的事务id，roll_pointer执行上一条undo日志）.  
![](vx_images/231500316230670.png)

从存储角度来看，undo日志会写入undo页中
undo日志还有一个作用，通过它和视图 可以实现MVCC（多版本并发控制）

### 5.2 redo日志
由innodb存储引擎支持。redo日志主要的作用用于支持事务的持久性。 bufferpool中的数据处于内存，如果异常导致崩溃，这里数据就会进行丢失。这里需要redo日志。原理是记录对xx页的xx偏移位置的做了xx操作，记录操作后的数据。 更新记录时，对bufferpool写，标记脏页，写redo日志。事务提交之后发生崩溃，通过redo日志恢复。 
注意，redo日志对应也有redobuffer.写磁盘也有几种策略，立即写入磁盘，写入文件（写文件和fsync到磁盘又不同，有pagecache）, 不写磁盘（后台线程每隔1s自动写入）
### 5.3 binlog日志
binlog日志由mysql server层支持。主要作用是用于主从复制和数据备份恢复。  从实现来说，每次进行sql操作，记录sql操作或者记录操作后的结果或者综合两者，会写入binlog cache，在事务提交时，cache将日志写入到binlog文件.写满另开binlog日志。(更新到磁盘时也有三种策略，立即刷新到磁盘，稍等刷新等等)
主从复制： 主库受到事务提交请求，先写binlog日志，在返回成功。 从库会开一个线程连接主库，用于接受binlog日志。 接收直接放入中继日志中。在主从复制模型中，有同步，异步，半同步等方式（取决于是否等从库接受binlog再返回成功给客户端）.

### 5.4 两阶段提交
在写binlog日志和写redo日志时，如果有一个失败，一个成功。会导致主从不一致。具体最开始链接内分析。这里通过事务为redo日志设立一个状态，redo日志先提交，处于准备状态。 在继续写binlog日志，再将redo日志设为完成状态。 数据恢复时根据redo日志状态中 内部事务id和 binlog日志是否有该内部事务id,来判断是否需要如何回滚等,用什么数据备份等。 详细点可能涉及binlog日志组等


## 6.事务
事务有四个特性：原子性（undo日志支持）、持久性（redo日志）、隔离性（MVCC），一致性（通过上述三种）。
上面章节通过日志讲述了原子性和持久性，这里主要介绍MVCC（Multi-Version COncurrentcy Control）-多版本并发控制。
多个事务同时对记录进行操作时，会产生如下几种情况
### 6.1 存在问题
* 脏读
    事务B读取了事务A在执行事务过程中的更高，之后事务A又进行了回滚。此时事务B读取错误数据。这是最严重事件
* 不可重复读
    在一个事务内多次读取同一个数据，如果出现前后两次读到的数据不一样的情况，就意味着发生了「不可重复读」现象。
* 幻读
    在一个事务内多次查询某个符合查询条件的「记录数量」，如果出现前后两次查询到的记录数量不一样的情况，就意味着发生了「幻读」现象。
### 6.2 隔离级别
针对上述3个情况，设立了4个隔离级别：
* 读未提交read uncommitted
    事务执行过程中，它对记录做的所有改动对其他事务可见，未解决上述任何问题
* 读提交read committed
    事务执行过程中，对记录的改变只有在当前事务提交之后才可对其他事务可见。解决了脏读问题。它主要依靠版本链（undo日志有讲，主要是事务id和rollpointer指针）和 readView来实现。主要依据是每次在执行sql过程之前，会先创建一个readview。根据readview状况判断要获取版本链中的哪个数据.
* 可重复读repeatable read
    它解决了脏读和不可重复读问题，在  事务开启后，创建一个readview，然后在整个事务执行过程中都使用这个readview. 它是默认设置的隔离级别。它无法解决幻读问题，通过记录锁+间隙锁可解决幻读问题.
* 串行化serializable
    解决了三个问题。每次对记录进行操作时，会对记录加读写锁。效率非常低。
    
### 6.3readview
从上可以看出，主要是通过readview和版本链来实现，区别是何时创建readview。
首先介绍下什么叫readview. 
![](vx_images/339994817230670.png)

在创建时主要记录上述数据，根据这个数据可版本链中的事务id. 可以判断出当前记录最新是被哪个事务执行的，该事务有无被提交。 根据这个，可以从版本链中找出对应的数据.

## 7.锁
锁有三个层次，全局锁、表级锁、行级锁。 

### 7.1全局锁
全局锁将整个数据库锁住，在此期间无法对数据库进行增删改等操作,数据库此时处于只可读状态. 常用用于全局备份。在备份期间开启全局锁。
命令为flush tables with read lock；解锁命令为unlock tables;
对于支持可重复读的innodb引擎，无法锁住整个数据库，可以在mysqldump时 开启事务，用于备份期间一致性

### 7.2 表级锁
#### 7.2.1 表锁
lock tables t_student read; 共享锁
lock tables t_student write;独占锁    
unlock tables
#### 7.2.2元数据锁MDL
不需要显示的使用元数据库，做crud操作会自动加MDL读锁，修改表结构时自动加MDL写锁。在事务提交后会自动释放锁（一条sql默认有一个事务）.MDL锁的等待会形成一个队列，队列中写锁优先级更高，所以MDL写锁阻塞后，后续读锁也会阻塞。

#### 7.2.3意向锁
在使用 InnoDB 引擎的表里对某些记录加上「共享锁/独占锁」之前，需要先在表级别加上一个「意向共享锁/意向独占锁]. 当执行插入、更新、删除操作，需要先对表加上「意向独占锁」，然后对该记录加独占锁. 它和表锁互斥，主要作用是插入表锁时快速判断表里是否有记录被加锁
普通查询sql使用MVCC快照，下面sql查询会进行加锁
select ... lock in share mode; //先在表上加上意向共享锁，然后对读取的记录加共享锁
select ... for update;//先表上加上意向独占锁，然后对读取的记录加独占锁

#### 7.2.4自增锁
自增键的需要用到这个，高版本还支持一个更轻量级锁，生成自增值后立即解锁。不必等待数据生成。此时使用这个轻量锁在主从模式下，binlog采用state模式会有数据不一致问题.

### 7.3 行锁
部分存储引起支持。innodb存储引擎支持行级锁. MyISAM不支持行级锁。 这是一种粒度更小的锁。
主要分为记录锁(锁单条记录)、间隙锁（左开右开区别）、next-key锁（记录锁+间隙锁，左开右闭）,.
记录锁好理解。select xx from xx where id = jj for update; 这时对这条记录加记录锁。
间隙锁这么理解。select xx from xx where id > jj for update; 对大于jj的数据，在每个数据上都会加上对应的锁。当插入数据时，判断待插入的后一个位置是否有对应的锁。 如果没直接插入。如果有则插入一个意向锁（生成锁结构并设置状态），陷入阻塞。

关于行锁，小林code有很多实例具体介绍了。
ps: