#include "MyUnitTest.h"

MyUnitTest::UnitTestMethod::UnitTestMethod()
{
	m_Result = UnitTestResult::Unkown;
}

MyUnitTest::UnitTestMethod::UnitTestMethod(const string& name, function<UnitTestResult()> action)
{
	m_Result = UnitTestResult::Unkown;
	m_Name = name;
	m_Content = action;
}

MyUnitTest::UnitTestGroup::UnitTestGroup(const string & name, const vector<UnitTestMethod>& list)
{
	m_Name = name;
	m_Content = list;
	GetUnitTestManager().AddUnitTestGroup(*this);
}

void MyUnitTest::UnitTestManager::AddUnitTestGroup(const UnitTestGroup & group)
{
	m_Content.insert(make_pair(group.m_Name,group));
}

void MyUnitTest::UnitTestManager::ListUnitTest()
{
	for (const auto& i : m_Content)
	{
		cout << i.first<<endl;
		for (const auto& j : i.second.m_Content)
		{
			cout << "\t" << j.m_Name;
			switch (j.m_Result)
			{
			case UnitTestResult::Unkown:
			{
				SetColor(SystemColorId::Yellow);
				cout << "\t" << "Unkown" << endl;
				SetColor();
				break;
			}
			case UnitTestResult::Pass:
			{
				SetColor(SystemColorId::Green);
				cout << "\t" << "Pass" << endl;
				SetColor();
				break;
			}
			case UnitTestResult::Fail:
			{
				SetColor(SystemColorId::Red);
				cout << "\t" << "Failed" << endl;
				SetColor();
				break;
			}
			default:
				break;
			}
		}
	}
}

void MyUnitTest::UnitTestManager::RunAllUnitTest()
{
	for (auto& i : m_Content)
	{
		RunUnitTestMethodInGroup(i.second);
	}
	TestSuccess();
}

void MyUnitTest::UnitTestManager::RunUnitTestMethod(const string & group_name, const string & name)
{
	auto iter = m_Content.find(group_name);
	if (iter != m_Content.end())
	{
		if (name != "*")
		{
			if (!RunUnitTestMethodInGroup(name, iter->second))
			{
				TestFailedWithNoMethod();
				return;
			}
		}
		else
		{
			RunUnitTestMethodInGroup(iter->second);
		}
	}
	else
	{
		TestFailedWithNoMethod();
		return;
	}
	TestSuccess();
}

void MyUnitTest::TestFailedWithNoMethod()
{
	SetColor(SystemColorId::Red);
	cout << "do not have this unit test method"<<endl;
	SetColor();
}

void MyUnitTest::TestSuccess()
{
	SetColor(SystemColorId::Green);
	cout << "test success" << endl;
	SetColor();
}

MyUnitTest::UnitTestManager& MyUnitTest::GetUnitTestManager()
{
	static MyUnitTest::UnitTestManager g_UnitTestManager;
	return g_UnitTestManager;
}

void MyUnitTest::RunUnitTestManager()
{
	UnitTestManager& manager = GetUnitTestManager();
	string cmd;
	manager.ListUnitTest();
	while (cin >> cmd && cmd != "exit")
	{
		system("cls");
		if (cmd == "list")
			manager.ListUnitTest();
		if (cmd == "run_all")
			manager.RunAllUnitTest();
		if (cmd == "run")
		{
			string gname, name;
			cin >> gname>>name;
			manager.RunUnitTestMethod(gname, name);
		}
	}
}

void MyUnitTest::SetColor(unsigned short ForeColor, unsigned short BackGroundColor)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, ForeColor + BackGroundColor * 0x10);
}

bool MyUnitTest::RunUnitTestMethod(UnitTestMethod & method)
{
	method.m_Result = method.m_Content();
	return true;
}

bool MyUnitTest::RunUnitTestMethodInGroup(UnitTestGroup & group)
{
	for (auto& i : group.m_Content)
	{
		RunUnitTestMethod(i);
	}
	return true;
}

bool MyUnitTest::RunUnitTestMethodInGroup(const string & name, UnitTestGroup & group)
{
	for (auto& i : group.m_Content)
	{
		if (i.m_Name == name)
		{
			RunUnitTestMethod(i);
			return true;
		}
	}
	return false;
}
