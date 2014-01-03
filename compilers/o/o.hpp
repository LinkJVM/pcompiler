#ifndef _O_HPP_
#define _O_HPP_

#include "pcompiler/base.hpp"

namespace Compiler
{
	class O : public Base
	{
	public:
		O();
		
		virtual OutputList transform(const QStringList& input, const Options& options, const kiss::KarPtr program = 0) const;
	private:
	};
}

#endif
