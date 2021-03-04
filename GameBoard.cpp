#include "GameBoard.h"

#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>

GameBoard::GameBoard(const size_t boardDimension, QObject *parent)
    : QAbstractListModel {parent},
      m_dimension {boardDimension},
      m_boardsize {m_dimension * m_dimension},
      m_hiddenElementValue {m_boardsize} //////////////////////////
{
    m_rawBoard.resize(m_boardsize);
    std::iota(m_rawBoard.begin(), m_rawBoard.end(),1);
    shuffle();
}

void GameBoard::shuffle()
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::mt19937 generator(seed);

    do{
        std::shuffle(m_rawBoard.begin(), m_rawBoard.end(), generator);
    }
    while (!isBoardValid());
}

bool GameBoard::isBoardValid() const
{
    int inv {0};
    for (size_t i {0}; i < m_boardsize; ++i) {
        for (size_t j = i; j < m_boardsize; ++j) {
            if (m_rawBoard[j].value < m_rawBoard[i].value && m_rawBoard[i].value!=m_boardsize){
                ++inv;
            }
        }
    }
    const size_t start_point = 1;

    for (size_t i = 0; i < m_boardsize; ++i) {
        if (m_rawBoard[i].value == m_boardsize){
            inv += start_point + i / m_dimension;
        }
    }

    return (inv % 2) == 0;
}

bool GameBoard::isPositionValid(const size_t position) const
{
    return position < m_boardsize;
}


int GameBoard::rowCount(const QModelIndex &/*parent*/) const
{
    return static_cast<int>(m_boardsize);
}

QVariant GameBoard::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole){
        return {};

    }
    const auto rowIndex {static_cast<size_t>(index.row())};

    if (!isPositionValid(rowIndex)){
        return {};
    }

    return QVariant(static_cast<int>(m_rawBoard[rowIndex].value));
}

size_t GameBoard::hiddenElementValue() const
{
    return m_hiddenElementValue;
}

GameBoard::Position GameBoard::getRowCol(size_t index) const
{
    Q_ASSERT(m_dimension > 0);
    size_t row = index / m_dimension;
    size_t column = index % m_dimension;

    return std::make_pair(row, column);
}

namespace  {
    bool isAdjasent (GameBoard::Position first, GameBoard::Position second)
    {
        if (first == second){
            return false;
        }

        const auto calc_distance = [](size_t pos1, size_t pos2){
            int distance = static_cast<int>(pos1);
            distance -= static_cast<int>(pos2);
            distance = std::abs(distance);
            return distance;
        };

        bool result {false};

        if (first.first == second.first) {
            int distance = calc_distance (first.second, second.second);
            if (distance == 1) {
                result = true;
            }
        } else if (first.second == second.second) {
            int distance = calc_distance (first.first, second.first);
            if (distance == 1) {
                result = true;
            }
        }

        return result;
    }
}

bool GameBoard::move(int index)
{
        if(!isPositionValid(static_cast<size_t>(index))) {
            return false;
        }

        const Position elementPosition {getRowCol(index)};

        auto hiddenElementIterator = std::find(m_rawBoard.begin(), m_rawBoard.end(),
                                               m_hiddenElementValue);
        Q_ASSERT(hiddenElementIterator != m_rawBoard.end());

        Position hiddenElementPositon {getRowCol(std::distance(m_rawBoard.begin(),
                                                                hiddenElementIterator))};

        if (!isAdjasent(elementPosition, hiddenElementPositon)){
            return false;
        }
        std::swap(hiddenElementIterator->value, m_rawBoard[index].value);
        //reset model is below, with emit (what is emit???)
        emit dataChanged(createIndex(0, 0), createIndex(m_boardsize, 0));//it is signal, it will be called when model is changed

        return true;
}

size_t GameBoard::dimension() const
{
    return m_dimension;
}





