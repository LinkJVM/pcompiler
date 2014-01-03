#ifndef _CPP_HPP_
#define _CPP_HPP_

#include "pcompiler/base.hpp"

namespace Compiler
{	
	class Java : public Base
	{
	public:
		Java();
		virtual OutputList transform(const QStringList& input, const Options& options, const kiss::KarPtr& program) const;
	private:
		static QString javacPath();
	};
}

#endif
