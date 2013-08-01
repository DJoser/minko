/*
Copyright (c) 2013 Aerys

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "minko/Common.hpp"

namespace minko
{
	namespace render
	{
		class ProgramInputs
		{
		public:
			typedef std::shared_ptr<ProgramInputs>	Ptr;

			enum class Type
			{
				unknown,
				int1,
				int2,
				int3,
				int4,
				float1,
				float2,
				float3,
				float4,
				float9,
				float16,
				sampler2d,
				attribute,
				constant
			};

		public:
			static
			Ptr
			create(std::shared_ptr<AbstractContext>	context,
				   const unsigned int 				program,
				   const std::vector<std::string>& 	names,
				   const std::vector<Type>& 		types,
				   const std::vector<unsigned int>& locations)
			{
				return std::shared_ptr<ProgramInputs>(new ProgramInputs(
					context,
					program,
					names,
					types,
					locations
				));
			}

			static
			std::string
			typeToString(const Type type)
			{
				switch (type)
				{
					case Type::unknown:
						return "unknown";
					case Type::int1:
						return "int1";
					case Type::int2:
						return "int2";
					case Type::int3:
						return "int3";
					case Type::int4:
						return "int4";
					case Type::float1:
						return "float1";
					case Type::float2:
						return "float2";
					case Type::float3:
						return "float3";
					case Type::float4:
						return "float4";
					case Type::float9:
						return "float9";
					case Type::float16:
						return "float16";
					default:
						throw std::invalid_argument("type");
				}
			}

		public:
			inline
			const std::vector<std::string>&
			names()
			{
				return _names;
			}

			inline
			const std::vector<Type>&
			types()
			{
				return _types;
			}

			inline
			const std::vector<unsigned int>&
			locations()
			{
				return _locations;
			}

			inline
			const int
			location(const std::string& name)
			{
				return _nameToLocation.count(name) ? _nameToLocation[name] : -1;
			}

		private:
			std::shared_ptr<AbstractContext>				_context;
			const unsigned int 								_program;
			std::vector<std::string>						_names;
			std::vector<Type> 								_types;
			std::vector<unsigned int>						_locations;
			std::unordered_map<std::string, unsigned int>	_nameToLocation;

		private:
			ProgramInputs(std::shared_ptr<AbstractContext>	context,
						  const unsigned int 				program,
						  const std::vector<std::string>& 	names,
						  const std::vector<Type>& 			types,
						  const std::vector<unsigned int>& 	locations) :
				_context(context),
				_program(program),
				_names(names),
				_types(types),
				_locations(locations)
			{
				for (unsigned int i = 0; i < _locations.size(); ++i)
					_nameToLocation[names[i]] = locations[i];
			}
		};
	}
}
