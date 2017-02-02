#pragma once
#include "minko/Common.hpp"
#include "minko/BulletCommon.hpp"
#include "minko/component/AbstractComponent.hpp"
#include "minko/scene/Layout.hpp"

namespace minko
{
	namespace component
	{
		namespace bullet
		{
			class Constraint
			{
			public:
				typedef std::shared_ptr<Constraint>                  Ptr;

			public:
				inline static
				Ptr
				create()
				{
					return std::shared_ptr<Constraint>(new Constraint());
				}

			private:
				Constraint();
				Constraint(Ptr a, Ptr b) {
				}
			};
		}
	}
}
