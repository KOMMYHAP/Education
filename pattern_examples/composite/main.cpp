/*
	"Ключом к паттерну компоновщик (composite) является абстрактный класс, который
	одновременно представляет и примитивы, и контейнеры" from "Design Pattern" 
	by Erich Gamma, John Vlissides, Ralph Johnson, and Richard Helm.
*/

#include "text.h"

int main()
{
	auto lSpace = makeSymbol(' '); // symbol ' '
	auto lComma = makeSymbol(','); // symbol ','
	auto lExcl = makeSymbol('!'); // symbol '!'
	auto lNewLine = makeSymbol('\n'); // symbol '\n'

	auto lH = makeSymbol('H'); // letter 'H'
	auto le = makeSymbol('e'); // letter 'e'
	auto ll = makeSymbol('l'); // letter 'l'
	auto lo = makeSymbol('o'); // letter 'o'
	auto lW = makeSymbol('W'); // letter 'W'
	auto lr = makeSymbol('r'); // letter 'r'
	auto ld = makeSymbol('d'); // letter 'd'

	auto wHello_ptr = makeCompositeText(); // word "Hello"
	wHello_ptr->add(lH);
	wHello_ptr->add(le);
	wHello_ptr->add(ll);
	wHello_ptr->add(ll);
	wHello_ptr->add(lo);

	auto wWorld_ptr = makeCompositeText(); // word "World"
	wWorld_ptr->add(lW);
	wWorld_ptr->add(lo);
	wWorld_ptr->add(lr);
	wWorld_ptr->add(ll);
	wWorld_ptr->add(ld);

	auto sentence_ptr = makeCompositeText();
	sentence_ptr->add(wHello_ptr);
	sentence_ptr->add(lComma);
	sentence_ptr->add(lSpace);
	sentence_ptr->add(wWorld_ptr);
	sentence_ptr->add(lExcl);
	sentence_ptr->add(lNewLine);

	sentence_ptr->print();

	auto li = makeSymbol('i'); // letter 'i'

	auto wHi_ptr = makeCompositeText(); // word "Hi"
	wHi_ptr->add(lH);
	wHi_ptr->add(li);

	sentence_ptr->replace(wHello_ptr, wHi_ptr);
	sentence_ptr->print();

	return 0;
}