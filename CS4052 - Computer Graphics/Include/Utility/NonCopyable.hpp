/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#pragma once


/*
Non-copyable base class.
This class prevents the creation of copy constructors and assignment operators in derived classes.
*/
class NonCopyable
{
public:
	NonCopyable() = default;
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator = (const NonCopyable&) = delete;
};
