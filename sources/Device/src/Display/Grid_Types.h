#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TYPE_GRID       (set.disp_typeGrid)
#define TYPE_GRID_1     (TYPE_GRID == Grid::Type::_1)
#define TYPE_GRID_2     (TYPE_GRID == Grid::Type::_2)
#define TYPE_GRID_3     (TYPE_GRID == Grid::Type::_3)
#define TYPE_GRID_4     (TYPE_GRID == Grid::Type::_4)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Grid
{
    /// ��� ����� �� ������.
    struct Type
    {
        enum E
        {
            _1,
            _2,
            _3,
            _4,
            Size
        } value;
    };
}
