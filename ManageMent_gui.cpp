#include "ManageMent_gui.h"
#include<sstream>
#include<fstream>
#include<string>
#include<iostream>
#include<algorithm>
#include"widgets/Window.h"
#include<conio.h>
#include"widgets/Timer.h"
#include<sstream>

using namespace std;

ManageMent::ManageMent():m_key(0),opt(66)
{
	readData("./resource/student.txt");

	m_bk = Image("./images/bk.jpg",Window::width(),Window::height());
	
	menu_btns.push_back(new PushButton("查看学生"));
	menu_btns.push_back(new PushButton("添加学生"));
	menu_btns.push_back(new PushButton("删除学生"));
	menu_btns.push_back(new PushButton("修改学生"));
	menu_btns.push_back(new PushButton("查找学生"));
	menu_btns.push_back(new PushButton("统计学生成绩"));
	menu_btns.push_back(new PushButton("按照总分排名"));
	menu_btns.push_back(new PushButton("退出系统"));
	for (int i = 0; i < menu_btns.size(); i++)
	{
		menu_btns[i]->setFixedSize(250, 50);
		menu_btns[i]->setHover(RED);

		int x = (Window::width() - menu_btns[i]->width()) / 2;
		int topMargin = (Window::height() - menu_btns.size() * menu_btns[0]->height()) / 2;
		int y = topMargin + i * menu_btns[i]->height();
		menu_btns[i]->move(x,y);
	}

	m_insertTable.setHeader(tableHeader);
	m_insertTable.move((Window::width()-m_insertTable.width())/2,150);
	m_insertTable.setRowCount(16);
	for (auto val : vec_stu)
	{
		m_insertTable.insert(val.formatInfo());
	}
	Font font = m_insertTable.font();
	font.setItalic(false);
	m_insertTable.setFont(font);


	m_insertEdit.setFixedSize(400, 30);
	m_insertEdit.move((Window::width() - m_insertEdit.width()) / 2, (Window::height() - m_insertEdit.height()) / 2);
	m_insertBtn.setFixedSize(50, 30);
	m_insertBtn.move(m_insertEdit.x() + m_insertEdit.width() + 20, m_insertEdit.y());
	m_insertBtn.setText("确定");

	//查找页面
	searchEdit = new LineEdit(0,0,200,30);
	searchIdBtn = new PushButton("按学号查找",0,0,200,30);
	searchNameBtn = new PushButton("按姓名查找", 0, 0, 200, 30);
	searchTable = new Table;
	searchTable->setRowCount(6);
	searchTable->setHeader(tableHeader);

	font = searchTable->font();
	font.setItalic(true);
	font.setHeight(20);
	searchTable->setFont(font);



	searchIdBtn->move((Window::width() - searchIdBtn->width()) / 2, Window::height() / 4);
	searchNameBtn->move((Window::width() - searchNameBtn->width()) / 2, Window::height() / 4 + searchNameBtn->height());
	searchEdit->move((Window::width() - searchEdit->width()) / 2, Window::height() / 3);
	searchTable->move((Window::width() - searchTable->width()) / 2, Window::height() / 3 + 100);

	//删除页面
	delEdit = new LineEdit(0, 0, 200, 30);
	delBtn = new PushButton("删除", 0, 0, 200, 30);
	delTable = new Table;
	delTable->setRowCount(6);
	delTable->setHeader(tableHeader);
	delEdit->setInputBoxTitle("请输入学生学号");

	delEdit->move((Window::width() - (delEdit->width()*2+20)) / 2, Window::height() / 3);
	delBtn->move(delEdit->x() + delEdit->width()+20, Window::height() / 3);
	delTable->move((Window::width() - delTable->width()) / 2, Window::height() / 3 + 100);

	// 修改页面
	modifyEdit = new LineEdit(0, 0, 200, 30);
	modifyBtn = new PushButton("查找", 0, 0, 200, 30);
	modifyTable = new Table;
	modifyNewDataEdit = new LineEdit(0, 0, 400, 30);
	modifyConfirmBtn = new PushButton("确认修改", 0, 0, 200, 30);

	modifyTable->setRowCount(6);
	modifyTable->setHeader(tableHeader);

	modifyEdit->setInputBoxTitle("请输入学生学号");
	modifyEdit->move((Window::width() - (modifyEdit->width() * 2 + 20)) / 2, Window::height() / 3);
	modifyBtn->move(modifyEdit->x() + modifyEdit->width() + 20, Window::height() / 3);
	modifyTable->move((Window::width() - modifyTable->width()) / 2, Window::height() / 3 + 100);
	modifyNewDataEdit->setInputBoxTitle("请输入新信息： < 学号 姓名 班级 数学 语文 英语>");
	modifyNewDataEdit->move((Window::width() - modifyNewDataEdit->width()) / 2, Window::height() / 3 + 200);
	modifyConfirmBtn->move((Window::width() - modifyConfirmBtn->width()) / 2, Window::height() / 3 + 250);

	//排名界面
	rankingTable.setHeader("名次\t学号\t姓名\t班级\t数学\t语文\t英语\t总分");	
	rankingTable.setRowCount(16); // 设置表格行数	
	rankingTable.setColCount(8); // 设置表格列数
	rankingTable.move((Window::width() - rankingTable.width()) / 2-115, 150); // 设置表格位置
}

void ManageMent::run()
{
	Window::beginDraw();
	while (true) {
		Window::clear();
		drawBackground();

		if (Window::hasMsg()) {
			m_msg = Window::getMsg();
			switch (m_msg.message) {
			case WM_KEYDOWN:
				switch (m_msg.vkcode) {
				case VK_ESCAPE:
					opt = 66; // 按下 ESC 返回主菜单
					break;
				}
				break;
			default:
				this->eventLoop(); // 处理事件
				break;
			}
		}

		// 根据 opt 的值显示不同界面
		switch (opt) {
		case 66:
			opt = mainMenu(m_msg); // 主菜单
			break;
		case ManageMent::ShowAll:
			displayAll();
			break;
		case ManageMent::Insert:
			insert();
			break;
		case ManageMent::Erase:
			erase();
			break;
		case ManageMent::Modify:
			modify();
			break;
		case ManageMent::Find:
			find();
			break;
		case ManageMent::Statistics:
			statistics();
			break;
		case ManageMent::Ranking:
			ranking();
			break;
		case ManageMent::Exit:
			writeData("./resource/student.txt");
			exit(666);
			break;
		default:
			break;
		}
		Window::flushDraw();
	}
	Window::endDraw();
}

void ManageMent::menu()
{
	for (int i = 0; i < menu_btns.size(); i++)
	{
		menu_btns[i]->show();
	}
}

void ManageMent::eventLoop()
{
	m_insertTable.eventLoop(m_msg);
	switch (opt) {
	case ManageMent::Insert:
		m_insertBtn.eventLoop(m_msg);
		m_insertEdit.eventLoop(m_msg);
		break;
	case ManageMent::Modify:
		modifyEdit->eventLoop(m_msg);
		modifyBtn->eventLoop(m_msg);
		modifyNewDataEdit->eventLoop(m_msg);
		modifyConfirmBtn->eventLoop(m_msg);
		break;
	case ManageMent::Find:
		searchIdBtn->eventLoop(m_msg);
		searchNameBtn->eventLoop(m_msg);
		searchEdit->eventLoop(m_msg);
		break;
	case ManageMent::Erase:
		delBtn->eventLoop(m_msg);
		delEdit->eventLoop(m_msg);
		break;
	case ManageMent::Ranking:
		rankingTable.eventLoop(m_msg);
		break;
	default:
		// 主菜单界面，不处理其他按钮事件
		break;
	}
}

void ManageMent::drawBackground()
{
	m_bk.draw();//绘制背景

}

int ManageMent::mainMenu(const ExMessage& msg)
{
	settextstyle(50, 0, "黑体");
	char title[50] = "学生成绩管理系统";
	outtextxy((Window::width() - textwidth(title)) / 2, 50, title);



	for (auto btn : menu_btns)
	{
		btn->show();
	}
	for (size_t i = 0; i < menu_btns.size(); i++)
	{
		menu_btns[i]->eventLoop(msg);
		if (menu_btns[i]->isClicked())
		{
			return i;
		}
	}
	return 66;
}

void ManageMent::updateTableData()
{
	m_insertTable.clear();
	for (auto val : vec_stu)
	{
		m_insertTable.insert(val.formatInfo());
	}
}

void ManageMent::displayAll()
{
	// 背景替换成白色
	setbkcolor(WHITE);
	cleardevice();

	// 设置标题
	settextstyle(50, 0, "黑体");
	char title[50] = "学生成绩列表";
	outtextxy((Window::width() - textwidth(title)) / 2, 50, title);

	// 显示表格
	m_insertTable.show();
}

void ManageMent::insert()
{
	settextstyle(50, 0, "黑体");
	char title[50] = "添加学生";
	outtextxy((Window::width() - textwidth(title)) / 2, 50, title);

	m_insertEdit.show();
	m_insertBtn.show();
	settextstyle(30, 0, "黑体");
	outtextxy(m_insertEdit.x()-50, m_insertEdit.y()-150, "请依次输入<学号,姓名,班级,数学,语文,英语>");

	//点一次会触发多次，所以用定时器来延迟一下
	if (m_insertBtn.isClicked() && Timer::startTimer(200,0))
	{
		std::string newData = m_insertEdit.text();
		if (newData.empty() || std::count(newData.begin(),newData.end(),' ')!=5)
		{
			printf("插入失败，字符串为空，或者格式不满足\n");
			return;
		}
		
		//格式化数据
		std::replace(newData.begin(), newData.end(), ' ', '\t');
		//把数据放入表格
		m_insertTable.insert(newData);	
		//把数据
		Student t;
		t.formatWrite(newData);
		vec_stu.push_back(t);
		m_insertEdit.clear();
		
		printf("插入成功啦\n");
	}

	
}

void ManageMent::erase()
{
	settextstyle(50, 0, "黑体");
	char title[50] = "删除学生";
	outtextxy((Window::width() - textwidth(title)) / 2, 50, title);

	delEdit->show();
	delBtn->show();
	delTable->show();

	static std::vector<Student>::iterator delIt = vec_stu.end();
	if (delEdit->textChanged())
	{
		std::string info = delEdit->text();
		if (info.empty())
			return;

		Student stu;
		stringstream stream(info);
		stream >> stu.number;
		delIt = std::find(vec_stu.begin(), vec_stu.end(), stu);
		if (delIt != vec_stu.end())
		{
			delTable->insert(delIt->formatInfo());
		}
	}
	
	//删除按钮按下
	if (delBtn->isClicked() && delIt != vec_stu.end() && Timer::startTimer(200,1))
	{
		std::cout << "delete" << std::endl;
		vec_stu.erase(delIt);
		delTable->clear();
		delIt = vec_stu.end();

		updateTableData();
	}
	
}


void ManageMent::modify()
{
	settextstyle(50, 0, "黑体");
	char title[50] = "修改学生信息";
	outtextxy((Window::width() - textwidth(title)) / 2, 50, title);

	// 提示用户输入格式
	settextstyle(20, 0, "黑体");
	outtextxy(modifyEdit->x() - 50, modifyEdit->y() - 50, "请输入学生学号以查找");

	modifyEdit->show();
	modifyBtn->show();
	modifyTable->show();
	modifyNewDataEdit->show();
	modifyConfirmBtn->show();

	static std::vector<Student>::iterator modifyIt = vec_stu.end();

	if (modifyBtn->isClicked())
	{
		std::string info = modifyEdit->text();
		if (info.empty())
			return;

		Student stu;
		stringstream stream(info);
		stream >> stu.number;
		modifyIt = std::find(vec_stu.begin(), vec_stu.end(), stu);
		if (modifyIt != vec_stu.end())
		{
			modifyTable->insert(modifyIt->formatInfo());
			/*bool m_showModifyControls = true;*/
		}
	}

	/*if (m_showModifyControls) {
		modifyNewDataEdit->show();
		modifyConfirmBtn->show();
	}*/

	
	if (modifyConfirmBtn->isClicked() && modifyIt != vec_stu.end())
	{
		std::string newData = modifyNewDataEdit->text();
		if (newData.empty() || std::count(newData.begin(), newData.end(), ' ') != 5)
		{
			return;
		}

		// 格式化数据
		std::replace(newData.begin(), newData.end(), ' ', '\t');
		modifyIt->formatWrite(newData);
		modifyTable->clear();
		modifyTable->insert(modifyIt->formatInfo());
		modifyNewDataEdit->clear();

		// 更新表格数据
		updateTableData();
		/*m_showModifyControls = false;*/

	}
	
}


void ManageMent::find()
{
	settextstyle(50, 0, "黑体", 0, 0, 0, 0, 0, 0);
	char title[50] = "查找学生";
	outtextxy((Window::width() - textwidth(title)) / 2, 50, title);
	
	int where = -1;		//记录时按学号查找还是按姓名查找
	if (searchIdBtn->isClicked())
	{
		std::cout << "numberBtn->isClickedQAQ~" << std::endl;
		searchEdit->setInputBoxTitle("请输入学号");
		searchEdit->popInputBox();
		where = 0;
	}
	if (searchNameBtn->isClicked())
	{
		searchEdit->setInputBoxTitle("请输入姓名");
		searchEdit->popInputBox();
		where = 1;
	}

	searchIdBtn->show();
	searchNameBtn->show();
	searchTable->show();
	

	std::string res = searchEdit->text();
	if (!res.empty())
	{
		Student stu;
		std::stringstream stream(res);
		if (where == 0)
		{
			stream >> stu.number ;
			auto findIt = std::find(vec_stu.begin(), vec_stu.end(), stu);
			if (findIt != vec_stu.end())
			{
				cout << "找到咯~ ";
				searchTable->insert(findIt->formatInfo());
				findIt->display();
			}
		}
		else if (where == 1)
		{
			stream >> stu.name;
			for (Student& s : vec_stu)
			{
				if (s.name == stu.name)
				{
					cout << "找到咯~ ";
					searchTable->insert(s.formatInfo());
					s.display();
				}
			}
		}
	}
	
}

void ManageMent::sort()
{
	//默认升序
	std::sort(vec_stu.begin(), vec_stu.end());
	std::cout << "排序成功QAQ~" << std::endl;
}

void ManageMent::calculateStatistics()
{
    // 初始化统计信息
    mathStats = { 0, 0.0, INT_MIN, INT_MAX, 0, 0, 0, 0, 0 };
    chineseStats = { 0, 0.0, INT_MIN, INT_MAX, 0, 0, 0, 0, 0 };
    englishStats = { 0, 0.0, INT_MIN, INT_MAX, 0, 0, 0, 0, 0 };

    for (const auto& student : vec_stu)
    {
        // 数学统计
        mathStats.total += student.math;
        mathStats.max = max(mathStats.max, student.math);
        mathStats.min = min(mathStats.min, student.math);
        if (student.math < 60) mathStats.failCount++;
        else if (student.math >= 60 && student.math < 70) mathStats.count60_69++;
        else if (student.math >= 70 && student.math < 80) mathStats.count70_79++;
        else if (student.math >= 80 && student.math < 90) mathStats.count80_89++;
        else if (student.math >= 90) mathStats.count90_100++;

        // 语文统计
        chineseStats.total += student.chinese;
        chineseStats.max = max(chineseStats.max, student.chinese);
        chineseStats.min = min(chineseStats.min, student.chinese);
        if (student.chinese < 60) chineseStats.failCount++;
        else if (student.chinese >= 60 && student.chinese < 70) chineseStats.count60_69++;
        else if (student.chinese >= 70 && student.chinese < 80) chineseStats.count70_79++;
        else if (student.chinese >= 80 && student.chinese < 90) chineseStats.count80_89++;
        else if (student.chinese >= 90) chineseStats.count90_100++;

        // 英语统计
        englishStats.total += student.english;
        englishStats.max = max(englishStats.max, student.english);
        englishStats.min = min(englishStats.min, student.english);
        if (student.english < 60) englishStats.failCount++;
        else if (student.english >= 60 && student.english < 70) englishStats.count60_69++;
        else if (student.english >= 70 && student.english < 80) englishStats.count70_79++;
        else if (student.english >= 80 && student.english < 90) englishStats.count80_89++;
        else if (student.english >= 90) englishStats.count90_100++;
    }

    // 计算平均分
    mathStats.average = static_cast<double>(mathStats.total) / vec_stu.size();
    chineseStats.average = static_cast<double>(chineseStats.total) / vec_stu.size();
    englishStats.average = static_cast<double>(englishStats.total) / vec_stu.size();
}

void ManageMent::statistics()
{	
	
	//背景替换成白色
	setbkcolor(WHITE);
	cleardevice();

	if (vec_stu.empty())
	{
		settextstyle(50, 0, "宋体");
		char title[50] = "暂无学生信息";
		outtextxy((Window::width() - textwidth(title)) / 2, 50, title);
		return;
	}

	//绘制表格
	setlinestyle(PS_SOLID, 2);
	line(190, 145, 1090, 145);
	for (int i = 1; i <= 9; i++)
	{
		line(190, 145 + i * 60, 1090, 145 + i * 60);
	}
	line(190, 145, 190, 685);
	for (int i = 1; i <= 4; i++) {
		line(190 + i * 225, 145, 190 + i * 225, 685);
	}

	//绘制表头
	outtextxy(490, 160, "语文");
	outtextxy(715, 160, "数学");
	outtextxy(940, 160, "英语");	
	outtextxy(250, 220, "平均分");
	outtextxy(250, 280, "最高分");
	outtextxy(250, 340, "最低分");
	outtextxy(225, 400, "不及格人数");
	outtextxy(205, 460, "60 - 69分人数");
	outtextxy(205, 520, "70 - 79分人数");
	outtextxy(205, 580, "80 - 89分人数");
	outtextxy(215, 640, "90分以上人数");


	// 设置标题样式和位置
	settextstyle(50, 0, "黑体");
	char title[50] = "学生成绩统计";
	int titleX = (Window::width() - textwidth(title)) / 2;
	int titleY = 50;
	outtextxy(titleX, titleY, title);

	// 计算统计信息
	calculateStatistics();

	// 设置文字显示位置
	int textX = 30;
    int textY=titleY + 110;
	int lineHeight = 60;

	// 设置文字样式
	settextstyle(30, 0, "宋体");

	// 显示数学统计信息
	//outtextxy(textX, textY, "数学成绩统计：");
	textY += lineHeight;
	outtextxy(460, textY, (std::to_string(mathStats.average)).c_str());
	textY += lineHeight;
	outtextxy(510, textY, (std::to_string(mathStats.max)).c_str());
	textY += lineHeight;
	outtextxy(520, textY, (std::to_string(mathStats.min)).c_str());
	textY += lineHeight;
	outtextxy(510, textY, (std::to_string(mathStats.failCount)).c_str());
	textY += lineHeight;
	outtextxy(510, textY, ( std::to_string(mathStats.count60_69)).c_str());
	textY += lineHeight;
	outtextxy(510, textY, (std::to_string(mathStats.count70_79)).c_str());
	textY += lineHeight;
	outtextxy(510, textY, (std::to_string(mathStats.count80_89)).c_str());
	textY += lineHeight;
	outtextxy(510, textY, ( std::to_string(mathStats.count90_100)).c_str());

	// 显示语文统计信息
	textX = 500;
	textY = titleY + 110;
	//outtextxy(textX, textY, "语文成绩统计：");
	textY += lineHeight;
	outtextxy(685, textY, (std::to_string(chineseStats.average)).c_str());
	textY += lineHeight;
	outtextxy(730, textY, (std::to_string(chineseStats.max)).c_str());
	textY += lineHeight;
	outtextxy(735, textY, ( std::to_string(chineseStats.min)).c_str());
	textY += lineHeight;
	outtextxy(735, textY, (std::to_string(chineseStats.failCount)).c_str());
	textY += lineHeight;
	outtextxy(735, textY, ( std::to_string(chineseStats.count60_69)).c_str());
	textY += lineHeight;
	outtextxy(735, textY, ( std::to_string(chineseStats.count70_79)).c_str());
	textY += lineHeight;
	outtextxy(735, textY, (std::to_string(chineseStats.count80_89)).c_str());
	textY += lineHeight;
	outtextxy(735, textY, (std::to_string(chineseStats.count90_100)).c_str());
	textY += lineHeight * 2;

	// 显示英语统计信息
	textX = 970;
	textY = titleY + 110;
	//outtextxy(textX, textY, "英语成绩统计：");
	textY += lineHeight;
	outtextxy(910, textY, (std::to_string(englishStats.average)).c_str());
	textY += lineHeight;
	outtextxy(950, textY, (std::to_string(englishStats.max)).c_str());
	textY += lineHeight;
	outtextxy(960, textY, (std::to_string(englishStats.min)).c_str());
	textY += lineHeight;
	outtextxy(960, textY, (std::to_string(englishStats.failCount)).c_str());
	textY += lineHeight;
	outtextxy(960, textY, ( std::to_string(englishStats.count60_69)).c_str());
	textY += lineHeight;
	outtextxy(960, textY, (std::to_string(englishStats.count70_79)).c_str());
	textY += lineHeight;
	outtextxy(960, textY, ( std::to_string(englishStats.count80_89)).c_str());
	textY += lineHeight;
	outtextxy(960, textY, ( std::to_string(englishStats.count90_100)).c_str());
}


void ManageMent::ranking()
{
	// 按总分降序排序
	std::sort(vec_stu.begin(), vec_stu.end(), [](const Student& a, const Student& b) {
		int totalA = a.math + a.chinese + a.english;
		int totalB = b.math + b.chinese + b.english;
		return totalA > totalB; // 降序排列
		});

	// 背景替换成白色
	setbkcolor(WHITE);
	cleardevice();

	// 设置标题
	settextstyle(50, 0, "黑体");
	char title[50] = "学生成绩排名（按总分降序）";
	outtextxy((Window::width() - textwidth(title)) / 2, 50, title);

	// 清空排名表格
	rankingTable.clear();

	// 分配名次并插入学生信息
	int rank = 1; // 当前名次
	for (size_t i = 0; i < vec_stu.size(); i++) {
		// 如果当前学生的总分与前一个学生不同，则更新名次
		if (i > 0) {
			int prevTotal = vec_stu[i - 1].math + vec_stu[i - 1].chinese + vec_stu[i - 1].english;
			int currTotal = vec_stu[i].math + vec_stu[i].chinese + vec_stu[i].english;
			if (currTotal < prevTotal) {
				rank = i + 1; // 更新名次
			}
		}

		// 插入学生信息
		stringstream ss;
		ss << rank << "\t" // 名次
			<< vec_stu[i].number << "\t" << vec_stu[i].name << "\t" << vec_stu[i].grade << "\t"
			<< vec_stu[i].math << "\t" << vec_stu[i].chinese << "\t" << vec_stu[i].english << "\t"
			<< (vec_stu[i].math + vec_stu[i].chinese + vec_stu[i].english); // 计算总分
		rankingTable.insert(ss.str());
	}

	rankingTable.setRowCount(16); // 设置表格行数
	//rankingTable.setColCount(8);
	// 显示排名表格
	rankingTable.show();
}

void ManageMent::readData(const std::string& fileName)
{
	fstream stream(fileName.c_str(), ios::in);
	if (!stream.is_open())
	{
		cerr << fileName << " file open failed" << endl;
		return;
	}
	Student stu;
	char buf[1024] = { 0 };
	//读取表头
	stream.getline(buf, 1024);
	tableHeader = buf;
	//读取数据
	while (!stream.eof())
	{
		memset(buf, 0, sizeof(buf));
		stream.getline(buf, 1024);
		//跳过空行
		if (strlen(buf) == 0)
			break;
		//格式化读取
		stringstream ss(buf);
		ss >> stu.number >> stu.name >> stu.grade >> stu.math >> stu.chinese >> stu.english;
		vec_stu.push_back(stu);
	}
	stream.close();
}

void ManageMent::writeData(const std::string& fileName)
{
	fstream write(fileName.c_str(), ios::trunc | ios::out);
	if (!write.is_open())
	{
		cerr << fileName << " file open failed [save]";
		return;
	}
	tableHeader += '\n';
	write.write(tableHeader.c_str(), tableHeader.size());
	for (auto& val : vec_stu)
	{
		string info = val.formatInfo();
		write.write(info.c_str(),info.size());
	}
	write.close();
}
