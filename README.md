SchoolPersonnelMgmt
===================

for exercise and homework

require Qt 5.1 and g++ 4.7+ for C++11 features

## 类结构

1. 数据
~~~text
	   -> Student -> Postgraduate
	  /							 \
Person							  -> TeachingAssistant(TA)
 	  \							 /
	   -> Teacher ---------------
~~~

2. 视图

由于`Q_OBJECTS`不支持多继承和类模板，所以只好分别为`Student`, `Postgraduate`, `Teacher`，`TeachingAssistant`定义TableModel
以Student Tab为例，包括：

 - `StudentTableModel`：提供数据，内部使用`QList<Student>`保存数据
 - `CommonTableDelegate`：双击单元格时`Sex`列的`QComboBox`和`Birthday`列的日期选择Widget由这个类提供。这个类可以处理所有的列，四个表格都使用这个类的实例。
 - `QTableView`：用于显示TableModel 中的内容。四个表格使用的都是这个类。

程序中定义了一个`enum`类型(`CONST::HDG`)，包含所有的列。在TableModel, TableDelegate, 过滤对话框中保存的都是这个类型的变量。这样做就不需要写四个TableDelegate, 四个过滤对话框了。还定义了一个静态方法 `CONST::getHeadingString()`，获得对应的列名，方便编程和国际化。类似的技巧也在`Person::Sex`中使用了，它定义了一个 `Person::getSexString()`静态方法来获取`Sex`enum类型变量对应的名称。

3. 过滤对话框

值得提一下的是选择一个日期范围对出生日期进行过滤。程序使用两个`QLineEdit`，选择的日期从A到B, 则QLineEdit A中在B的当前显示的日期之后的日期都应当变成灰色，处于不可选状态；QLineEdit B中在A的当前显示的日期之前的日期也应变成灰色，以免用户选到这些日期。维持A的最大可选日期和B的最小可选日期可以通过A与B互相接收对方的日期变化的信号来实现。下面程序中的 if 判断防止A,B不断互发信号陷入死循环。

~~~cpp
connect(this->fromDate_, &QDateEdit::dateChanged, [this](const QDate &date){
	if(date != this->toDate_->minimumDate()){
		this->toDate_->setMinimumDate(date);
	}
});
connect(this->toDate_, &QDateEdit::dateChanged, [this](const QDate &date){
	if(date != this->fromDate_->maximumDate()){
		this->fromDate_->setMaximumDate(date);
	}
});
~~~

用户每输入一个数字，或者改变一个选项就发相应信号。QTableView 收到信号后就调用 TableModel 的sort方法进行过滤。

实现过滤的方法是将符合过滤条件的数据放进一个新的QList, 然后把TableModel内部的QList<>替换为这个新的list. 为了在关闭过滤对话框后恢复原来的数据，打开过滤对话框时把原来的 list 备份到 listOld_中，关闭对话框时恢复。

使用这种方法，过滤过程中对表格数据进行的修改将在过滤对话框关闭后丢失，因此禁止在过滤对话框打开的状态下修改表格内容。方法是对相应的TableView设置`setEditTriggers(QAbstractItemView::NoEditTriggers);`，进行设置前先保存原来的EditTriggers的值，过滤对话框关闭时恢复原来的值。

支持使用Perl风格的正则表达式(PCRE)对字符串进行过滤。

4. 排序

单击表格的列名就可按相应的列进行排序。程序实现中使用`qStableSort()`进行排序。其中对`Name`字段假定其中的字符串大多是中文，按照中文拼音排序（将QString转为std::string,然后使用`std::collate.compare()`比较两个字符串的大小（按照中文locale）, 参见我的博文[sort Chinese characters with C++ | C++按照拼音对中文排序](http://ssendeavour.github.io/2013/10/25/sort-Chinese-characters-in-cpp))。

5. 保存读取文件

保存文件时，首先写入 `magic number`, 然后写入文件类型（Student，Teacher, Postgraduate, TeachingAssistant分别使用四种不同的文件类型），再写入文件版本。之后写入要保存的数据数量（QList的大小），然后才是真正的数据。

读取时按照写入的顺序作相应的检查和处理。

## 不足

1. 类设计的不足。由于Qt的QObjects这一套体系不支持多继承和类模板，因此不得不为四种数据分别编写TableModel。`Person`类和其子类也无法从`QObject`那里继承。

2. 实现的不足:
	- 过滤功能实现的太过简单粗暴了，而且用户每输入或删除一个字符都会重新过滤，数据量大时这样做效率很低。
	- 没有进行足够的错误处理。如果数据量非常大，使用QList应该会出问题的（数据量大了使用任何方法都会内存不够），可是不知道会出什么错误，不知道会不会抛异常。

