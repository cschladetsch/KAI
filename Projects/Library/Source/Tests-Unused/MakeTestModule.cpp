
void RegisterTestSuites(Test::Module &module)
{
	module.AddSuite<Tests::TestRegistry>("TestRegistry");
	module.AddSuite<Tests::TestPathname>("TestPathname");
	module.AddSuite<Tests::TestObject>("TestObject");
	module.AddSuite<Tests::TestFunction>("TestFunction");
	module.AddSuite<Tests::TestTree>("TestTree");
	module.AddSuite<Tests::TestCompiler>("TestCompiler");
	module.AddSuite<Tests::TestExecutor>("TestExecutor");
	module.AddSuite<Tests::TestMethod>("TestMethod");
	module.AddSuite<Tests::TestBinaryStream>("TestBinaryStream");
	module.AddSuite<Tests::TestMap>("TestMap");
}

//EOF
