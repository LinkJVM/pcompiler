#include "class.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "pcompiler/compiler_options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>
#include <QProcessEnvironment>

using namespace Compiler;

Class::Class()
	: Base("jar", QStringList() << "class", 1, QStringList() << OUTPUT_DIR)
{
}

OutputList Class::transform(const QStringList& input, Options& options, const kiss::KarPtr& program) const
{
	Output ret;
	ret.setFiles(input);
	
	QProcess compiler;
	QProcessEnvironment compilerEnvironment;
	
	compilerEnvironment.insert("BOOTCLASSPATH", "/usr/share/jamvm/classes.zip:/usr/share/classpath/glibj.zip:/usr/lib/linkjvm-java.jar:/usr/share/classpath/tools.zip");
	compilerEnvironment.insert("CLASSPATH", "/usr/share/jamvm/classes.zip:/usr/share/classpath/glibj.zip:/usr/lib/linkjvm-java.jar:/usr/share/classpath/tools.zip:.");
	compilerEnvironment.insert("LD_LIBRARY_PATH", "/usr/lib/classpath");
		
	compiler.setProcessEnvironment(compilerEnvironment);
	
	QString output = (options.contains(OUTPUT_DIR) ? options[OUTPUT_DIR] + ".jar" : QFileInfo(input[0]).absolutePath() + "/a.jar");
	if(program->hasPreference("main_class")){
		compiler.start(jarPath(), (QStringList() << "cvef" << program->getPreference("main_class") << output) + input);
	}
	else{
		compiler.start(jarPath(), (QStringList() << "cvf" << output) + input);
	}
	if(!compiler.waitForStarted()) {
		ret = Output(Platform::ccPath(), 1, "", "error: couldn't start the java archiver");
		return OutputList() << ret;
	}
	compiler.waitForFinished();
	
	ret.setExitCode(compiler.exitCode());
	ret.setOutput(compiler.readAllStandardOutput());
	ret.setError(compiler.readAllStandardError());
	ret.setGeneratedFiles(QStringList() << output);
	ret.setTerminal(true);
	return OutputList() << ret;
}

QString Class::jarPath()
{
	return "jar";
}

REGISTER_COMPILER(Class)