/*=========================================================================

  Program:   ImagingEngine
  Module:    transfer_function_template.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once
#include <list>

using namespace std;

namespace DW {
	namespace Render {
				
		// Forward declare the class template
		template <typename T>
		class TransferFunctionTemplate;

		///// Forward Declare the stream operators
		///// see http://web.mst.edu/~nmjxv3/articles/templates.html and http://en.cppreference.com/w/cpp/language/friend
		//template <typename T>
		//ostream & operator <<(ostream  &stream, const TransferFunctionTemplate<T> &transferFunction);
		//template <typename T>
		//ostream & operator >>(ostream  &stream, TransferFunctionTemplate<T> &transferFunction);

		/**
		It Represents a basic transfer function f: X-> Y, where X is a set of actuals (properties or magnitude of the gradient)
		And Y a set of T-type values.
		This function has a few explicitly defined points and the rest is obtained by linear interpolation or extrapolation of the closest neighbor.
		The transfer function also has a name.
		This template has the protected constructor, so it cannot be instantiated directly, but a subclass must be made to
		Each type T you want.
		*/
		template <typename T>
		class TransferFunctionTemplate 
		{
		public:
			///// Returns true if the names and functions are equal.
			///// \note functions can be different but equivalent.They Are equivalent if once simplified they are equal.
			//bool operator ==(const TransferFunctionTemplate<T> &transferFunction) const;
			/// Returns the name.
			const string& name() const;
			/// Assign name.
			void setName(const string &name);
			/// Returns the corresponding x.
			T operator ()(double x) const;
			/// Returns the corresponding x.
			T get(double x) const;
			/// Explicitly Defines the dot (x, y).
			void set(double x, const T &y);
			/// Erases the explicit definition of the dot (x, y) if it exists.
			void unset(double x);
			/// Deletes all explicitly defined points.
			void clear();
			/// Returns true if there is a dot (x, y) explicitly defined, and false otherwise.
			bool isSet(double x) const;

			/// Returns true if this transfer function doesn't have any point defined.
			bool isEmpty() const;

			/// Returns the list of x values of all dots (x, y) explicitly defined.
			list<double> keys() const;

			/// It Cuts the function so that it only has explicit points in the range [x1, X2] and has explicit points in X1 and X2.
			void trim(double x1, double x2);
			/// It Simplifies the function by erding the dots (x, y) that can be obtained by interpolation or extrapolation.
			void simplify();

			///// Writes the given transfer function to the given stream.
			//friend ostream& operator << <>(ostream  &stream, const TransferFunctionTemplate &transferFunction);
			///// Fills the given transfer function from the given stream.
			//friend ostream & operator >> <>(ostream  &stream, TransferFunctionTemplate &transferFunction);

		protected:
			/// Creates an empty transfer function, without any point and with no name.
			TransferFunctionTemplate();

		protected:
			/// Name of the transfer function.
			string m_name;
			/// Map of the explicitly defined dots (x, y).
			map<double, T> m_map;

		private:

			/// Returns a default value of type T.
			static T defaultValue();
			/// Returns the linear interpolation between A and B evaluating "A + Alpha * (b-a)", which is equivalent to "a * (1-alpha) + b * alpha".
			static T linearInterpolation(const T &a, const T &b, double alpha);

		};

		//template <typename T>
		//bool TransferFunctionTemplate<T>::operator ==(const TransferFunctionTemplate<T> &transferFunction) const
		//{
		//	return m_name == transferFunction.m_name && m_map == transferFunction.m_map;
		//}

		template <typename T>
		const string& TransferFunctionTemplate<T>::name() const
		{
			return m_name;
		}

		template <typename T>
		void TransferFunctionTemplate<T>::setName(const string &name)
		{
			m_name = name;
		}

		template <typename T>
		T TransferFunctionTemplate<T>::operator ()(double x) const
		{
			return get(x);
		}

		template <typename T>
		T TransferFunctionTemplate<T>::get(double x) const
		{
			if (m_map.isEmpty())
			{
				return defaultValue();
			}

			typename map<double, T>::const_iterator lowerBound = m_map.lowerBound(x);

			// > Last
			if (lowerBound == m_map.end())
			{
				return (--lowerBound).value();
			}

			// Exact or < First
			if (lowerBound.key() == x || lowerBound == m_map.begin())
			{
				return lowerBound.value();
			}

			typename map<double, T>::const_iterator a = lowerBound - 1, b = lowerBound;
			double alpha = (x - a.key()) / (b.key() - a.key());

			return linearInterpolation(a.value(), b.value(), alpha);
		}

		template <typename T>
		void TransferFunctionTemplate<T>::set(double x, const T &y)
		{
			// Problem with QTransferFunctionEditorByGradient
			// Q_ASSERT(!MathTools::isNaN(x));
			m_map[x] = y;
		}

		template <typename T>
		void TransferFunctionTemplate<T>::unset(double x)
		{
			m_map.remove(x);
		}

		template <typename T>
		void TransferFunctionTemplate<T>::clear()
		{
			m_map.clear();
		}

		template <typename T>
		bool TransferFunctionTemplate<T>::isSet(double x) const
		{
			return m_map.contains(x);
		}

		template <typename T>
		bool TransferFunctionTemplate<T>::isEmpty() const
		{
			return m_map.isEmpty();
		}

		template <typename T>
		list<double> TransferFunctionTemplate<T>::keys() const
		{
			return m_map.keys();
		}

		template <typename T>
		void TransferFunctionTemplate<T>::trim(double x1, double x2)
		{
			if (m_map.isEmpty())
			{
				return;
			}

			// First we add the ends. If they already exist it will remain the same.
			set(x1, get(x1));
			set(x2, get(x2));

			//// We Then remove the points outside the range.
			//map<string,int>::iterator it;
			//it = m_map.begin();
			//while(it != m_map.end())
			//{
			//	//it->first;
			//	//it->second;
			//	if (it->first < x1)
			//	{
			//		it
			//	}
			//	else
			//	{
			//		break;
			//	}
			//	it ++;         
			//}

			//QMutableMapIterator<double, T> it(m_map);
			//while (it.hasNext())
			//{
			//	it.next();
			//	if (it.key() < x1)
			//	{
			//		it.remove();
			//	}
			//	else
			//	{
			//		break;
			//	}
			//}
			//it.toBack();
			//while (it.hasPrevious())
			//{
			//	it.previous();
			//	if (it.key() > x2)
			//	{
			//		it.remove();
			//	}
			//	else
			//	{
			//		break;
			//	}
			//}
		}

		template <typename T>
		void TransferFunctionTemplate<T>::simplify()
		{
			//list<double> keys = m_map.keys();

			//foreach (double x, keys)
			//{
			//	if (m_map.size() == 1)
			//	{
			//		// Cas especial: si nom¨¦s queda un punt no l'hem de treure
			//		break;
			//	}
			//	T y = get(x);
			//	unset(x);
			//	if (get(x) != y)
			//	{
			//		set(x, y);
			//	}
			//}
		}

		//template <typename T>
		//ostream & operator <<(ostream  &stream, const TransferFunctionTemplate<T> &transferFunction)
		//{
		//	return stream << transferFunction.m_name << transferFunction.m_map;
		//}

		//template <typename T>
		//ostream & operator >>(ostream  &stream, TransferFunctionTemplate<T> &transferFunction)
		//{
		//	return stream >> transferFunction.m_name >> transferFunction.m_map;
		//}

		template <typename T>
		TransferFunctionTemplate<T>::TransferFunctionTemplate()
		{
		}

		template <typename T>
		T TransferFunctionTemplate<T>::defaultValue()
		{
			return T();
		}

		template <typename T>
		T TransferFunctionTemplate<T>::linearInterpolation(const T &a, const T &b, double alpha)
		{
			return a + alpha * (b - a);
		}

	}
}
