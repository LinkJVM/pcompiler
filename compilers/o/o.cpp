#include "o.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "../common/options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>

using namespace Compiler;

#define O_FLAGS "LD_FLAGS"

O::O()
	: Base("ld (c++)", QStringList() << "o", 1, QStringList() << O_FLAGS << OUTPUT_DIR)
{
}

OutputList O::transform(const QStringList& input, const Options& options) const
{
	Output ret;
	ret.setFiles(input);
	
	QProcess linker;
	const QString& ext = Platform::exeExtension();
	QString executableName = input.size() == 1 ? QFileInfo(input[0]).baseName() : "application";
	QString output = (options.contains(OUTPUT_DIR) ? options[OUTPUT_DIR] : QFileInfo(input[0]).absolutePath())
		+ "/" + executableName + (ext.isEmpty() ? "" : "." + ext);
	
	QString rawFlags = options[O_FLAGS].trimmed();
	QStringList flags = rawFlags.isEmpty() ? QStringList() : rawFlags.split(" ");
	linker.start(Platform::cppPath(), (flags + input) << "-o" << output);
	linker.waitForStarted();
	linker.waitForFinished();
	
	ret.setExitCode(linker.exitCode());
	ret.setOutput(linker.readAllStandardOutput());
	ret.setError(linker.readAllStandardError());
	ret.setGeneratedFiles(QStringList() << output);
	ret.setTerminal(true);
	
	return OutputList() << ret;
}

REGISTER_COMPILER(O)