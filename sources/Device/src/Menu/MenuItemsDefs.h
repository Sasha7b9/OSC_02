#pragma once
#include "defines.h"
#include "Utils/CommonFunctions.h"


typedef bool(*pFuncBKE)(KeyEvent);

inline bool EmptyFuncfBKE(KeyEvent) { return false; }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_BUTTON(name, title, hint, keeper, funcActive, funcPress, funcDraw)                                                                                                              \
static const DataButton db##name = {funcPress, funcDraw};                                                                                                                                   \
static const DataItem di##name = { Item::Type::Button, title, hint, 0, keeper, funcActive, &db##name };                                                                                     \
static const Button name(&di##name);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GOVERNOR(name, title, hint, cell, min, max, keeper, funcActive, funcChanged, funcBeforeDraw)                                                                                    \
static const DataGovernor dg##name = {(int16 *)&cell, min, max, funcChanged, funcBeforeDraw};                                                                                               \
static const DataItem di##name = { Item::Type::Button, title, hint, 0, keeper, funcActive, &dg##name };                                                                                     \
static const Governor name(&di##name);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GOVERNOR_COLOR(name, title, hint, colorType, keeper)                                                                                                                            \
static const DataGovernorColor dgc##name = {&colorType};                                                                                                                                    \
static const DataItem di##name = { Item::Type::GovernorColor, title, hint, 0, keeper, EmptyFuncBtV, &dgc##name };                                                                           \
static const GovernorColor name(&di##name);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GRAPH_BUTTON(name, title, hint, keeper, funcActive, funcPress, funcDraw)                                                                                                        \
static const DataGraphButton dgb##name = {funcPress, funcDraw, nullptr};                                                                                                                    \
static const DataItem hi##name = { Item::Type::GovernorColor, title, hint, 0, keeper, funcActive, &dgb##name };                                                                             \
static const GraphButton name(&hi##name);

#define DEF_GRAPH_BUTTON_HINTS_2(name, title, hint, keeper, funcActive, funcPress, funcDraw, FuncDrawHint1, hint1, FuncDrawHint2, hint2)                                                    \
static const StructHelpDrawButton h##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 }};                                                                                         \
static const DataGraphButton dgb##name = {funcPress, funcDraw, h##name};                                                                                                                    \
static const DataItem hi##name = { Item::Type::GovernorColor, title, hint, 2, keeper, funcActive, &dgb##name };                                                                             \
static const GraphButton name(&hi##name);

#define DEF_GRAPH_BUTTON_HINTS_3(name, title, hint, keeper, funcActive, funcPress, funcDraw, FuncDrawHint1, hint1, FuncDrawHint2, hint2, FuncDrawHint3, hint3)                              \
static const StructHelpDrawButton h##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 }, { FuncDrawHint3, hint3 } };                                                              \
static const DataGraphButton dgb##name = {funcPress, funcDraw, h##name};                                                                                                                    \
static const DataItem hi##name = { Item::Type::GovernorColor, title, hint, 3, keeper, funcActive, &dgb##name };                                                                             \
static const GraphButton name(&hi##name);

#define DEF_GRAPH_BUTTON_HINTS_5(name, title, hint, keeper, funcActive, funcPress, funcDraw,                                                                                                \
    FuncDrawHint1, hint1, FuncDrawHint2, hint2, FuncDrawHint3, hint3, FuncDrawHint4, hint4, FuncDrawHint5, hint5)                                                                           \
static const StructHelpDrawButton h##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 },                                                                                          \
    { FuncDrawHint3, hint3 }, { FuncDrawHint4, hint4 }, { FuncDrawHint5, hint5 } };                                                                                                         \
static const DataGraphButton dgb##name = {funcPress, funcDraw, h##name};                                                                                                                    \
static const DataItem hi##name = { Item::Type::GovernorColor, title, hint, 5, keeper, funcActive, &dgb##name };                                                                             \
static const GraphButton name(&hi##name);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_PAGE_1(name, title, hint, item1, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                                 \
static const Item * const i##name[] = {(Item *)item1};                                                                                                                                      \
static const DataPage dp##name = { namePage, i##name, funcPress, funcDraw, funcRegSet };                                                                                                    \
static const DataItem di##name = { Item::Type::Page, title, hint, 1, keeper, funcActive, &dp##name };                                                                                       \
static const Page name(&di##name);

#define DEF_PAGE_2(name, title, hint, item1, item2, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                          \
static const Item * const  i##name[] = {(Item *)item1, (Item *)item2};                                                                                                                      \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                                                                      \
static const DataItem di##name = { Item::Type::Page, title, hint, 2, keeper, funcActive, &dp##name };                                                                                       \
static const Page name(&di##name);

#define DEF_PAGE_3(name, title, hint, item1, item2, item3, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                   \
static const Item * const i##name[] = {(Item *)(item1), (Item *)(item2), (Item *)(item3)};                                                                                                  \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                                                                      \
static const DataItem di##name = { Item::Type::Page, title, hint, 3, keeper, funcActive, &dp##name};                                                                                        \
static const Page name(&di##name);

#define DEF_PAGE_4(name, title, hint, item1, item2, item3, item4, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                            \
static const Item * const i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4};                                                                                         \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                                                                      \
static const DataItem di##name = { Item::Type::Page, title, hint, 4, keeper, funcActive, &dp##name };                                                                                       \
static const Page name(&di##name);

#define DEF_PAGE_5(name, title, hint,  item1, item2, item3, item4, item5, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                    \
static const Item * const i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5};                                                                          \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                                                                      \
static const DataItem di##name = { Item::Type::Page, title, hint, 5, keeper, funcActive, &dp##name };                                                                                       \
static const Page name(&di##name);

#define DEF_PAGE_5_VAR(name, title, hint,  item1, item2, item3, item4, item5, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                \
static const Item *i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5};                                                                                 \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                                                                      \
static const DataItem di##name = { Item::Type::Page, title, hint, 5, keeper, funcActive, &dp##name };                                                                                       \
static const Page name(&di##name);

#define DEF_PAGE_6(name, title, hint, item1, item2, item3, item4, item5, item6, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                              \
static const Item * const i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6};                                                           \
static const DataPage dp##name = { namePage, i##name, funcPress, funcDraw, funcRegSet};                                                                                                     \
static const DataItem di##name = { Item::Type::Page, title, hint, 6, keeper, funcActive, &dp##name };                                                                                       \
static const Page name(&di##name);

#define DEF_PAGE_7(name, title, hint, item1, item2, item3, item4, item5, item6, item7, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                       \
static const Item * const  i##name[] = { (Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6, (Item *)item7};                                          \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                                                                      \
static const DataItem di##name = { Item::Type::Page, title, hint, 7, keeper, funcActive, &dp##name };                                                                                       \
static const Page name(&di##name);

#define DEF_PAGE_8(name, title, hint, item1, item2, item3, item4, item5, item6, item7, item8, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                \
static const Item * const  i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6, (Item *)item7, (Item *)item8};                            \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                                                                      \
static const DataItem di##name = { Item::Type::Page, title, hint, 8, keeper, funcActive, &dp##name };                                                                                       \
static const Page name(&di##name);

#define DEF_PAGE_12(name, title, hint, item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)\
static const Item * const  i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5,                                                                          \
    (Item *)item6, (Item *)item7, (Item *)item8, (Item *)item9, (Item *)item10, (Item *)item11, (Item *)item12};                                                                            \
static const DataPage dp##name = {namePage, i##name, funcPress, funcDraw, funcRegSet};                                                                                                      \
static const DataItem di##name = { Item::Type::Page, title, hint, 12, keeper, funcActive, &dp##name };                                                                                      \
static const Page name(&di##name);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_CHOICE_2(name, title, hint, name1, name2, cell, keeper, funcActive, funcChanged, funcDraw)                                                                                      \
static const pString n##name[] = {name1, name2};                                                                                                                                            \
static const DataChoice dc##name[] = {(int8 *)&cell, n##name, funcChanged, funcDraw};                                                                                                       \
static const DataItem di##name = { Item::Type::Choice, title, hint, 2, keeper, funcActive, &dc##name };                                                                                     \
static const Choice name(&di##name);

#define DEF_CHOICE_3(name, title, hint, name1, name2, name3, cell, keeper, funcActive, funcChanged, funcDraw)                                                                               \
static const pString n##name[] = {name1, name2, name3};                                                                                                                                     \
static const DataChoice dc##name[] = {(int8 *)&cell, n##name, funcChanged, funcDraw};                                                                                                       \
static const DataItem di##name = { Item::Type::Choice, title, hint, 3, keeper, funcActive, &dc##name };                                                                                     \
static const Choice name(&di##name);

#define DEF_CHOICE_4(name, title, hint, name1, name2, name3, name4, cell, keeper, funcActive, funcChanged, funcDraw)                                                                        \
static const pString n##name[] = {name1, name2, name3, name4};                                                                                                                              \
static const DataChoice dc##name[] = {(int8 *)&cell, n##name, funcChanged, funcDraw};                                                                                                       \
static const DataItem di##name = { Item::Type::Choice, title, hint, 4, keeper, funcActive, &dc##name };                                                                                     \
static const Choice name(&di##name);

#define DEF_CHOICE_5(name, title, hint, name1, name2, name3, name4, name5, cell, keeper, funcActive, funcChanged, funcDraw)                                                                 \
static const pString n##name[] = {name1, name2, name3, name4, name5};                                                                                                                       \
static const DataChoice dc##name[] = {(int8 *)&cell, n##name, funcChanged, funcDraw};                                                                                                       \
static const DataItem di##name = { Item::Type::Choice, title, hint, 5, keeper, funcActive, &dc##name };                                                                                     \
static const Choice name(&di##name);

#define DEF_CHOICE_6(name, title, hint, name1, name2, name3, name4, name5, name6, cell, keeper, funcActive, funcChanged, funcDraw)                                                          \
static const pString n##name[] = {name1, name2, name3, name4, name5, name6};                                                                                                                \
static const DataChoice dc##name[] = {(int8 *)&cell, n##name, funcChanged, funcDraw};                                                                                                       \
static const DataItem di##name = { Item::Type::Choice, title, hint, 6, keeper, funcActive, &dc##name };                                                                                     \
static const Choice name(&di##name);

#define DEF_CHOICE_7(name, title, hint, name1, name2, name3, name4, name5, name6, name7, cell, keeper, funcActive, funcChanged, funcDraw)                                                   \
static const pString n##name[] = {name1, name2, name3, name4, name5, name6, name7};                                                                                                         \
static const DataChoice dc##name[] = {(int8 *)&cell, n##name, funcChanged, funcDraw};                                                                                                       \
static const DataItem di##name = { Item::Type::Choice, title, hint, 7, keeper, funcActive, &dc##name };                                                                                     \
static const Choice name(&di##name);

#define DEF_CHOICE_9(name, title, hint, name1, name2, name3, name4, name5, name6, name7, name8, name9, cell, keeper, funcActive, funcChanged, funcDraw)                                     \
static const pString n##name[] = {name1, name2, name3, name4, name5, name6, name7, name8, name9};                                                                                           \
static const DataChoice dc##name = {(int8 *)&cell, n##name, funcChanged, funcDraw};                                                                                                         \
static const DataItem di##name = { Item::Type::Choice, title, hint, 9, keeper, funcActive, &dc##name };                                                                                     \
static const Choice name(&di##name);

#define DEF_CHOICE_10(name, title, hint, name1, name2, name3, name4, name5, name6, name7, name8, name9, name10, cell, keeper, funcActive, funcChanged, funcDraw)                            \
static const pString n##name[] = {name1, name2, name3, name4, name5, name6, name7, name8, name9, name10};                                                                                   \
static const DataChoice dc##name = {(int8 *)&cell, n##name, funcChanged, funcDraw};                                                                                                         \
static const DataItem di##name = { Item::Type::Choice, title, hint, 10, keeper, funcActive, &dc##name };                                                                                    \
static const Choice name(&di##name);


#define DEF_CHOICE_16(name, title, hint, name1, name2,  name3,  name4,  name5,  name6,  name7,  name8, name9, name10,                                                                       \
    name11, name12, name13, name14, name15, name16, cell, keeper, funcActive, funcChanged, funcDraw)                                                                                        \
static const pString n##name[] = {name1,  name2, name3,  name4,  name5,  name6, name7, name8, name9, name10, name11, name12, name13, name14, name15, name16 };                              \
static const DataChoice dc##name = {(int8 *)&cell, n##name, funcChanged, funcDraw};                                                                                                         \
static const DataItem di##name = { Item::Type::Choice, title, hint, 16, keeper, funcActive, &dc##name };                                                                                    \
static const Choice name(&di##name);
