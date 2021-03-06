/***************************************************************************
**                                                                        **
**  Polyphone, a soundfont editor                                         **
**  Copyright (C) 2013-2014 Davy Triponney                                **
**                2014      Andrea Celani                                 **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Davy Triponney                                       **
**  Website/Contact: http://www.polyphone.fr/                             **
**             Date: 01.01.2013                                           **
***************************************************************************/

#include "circularbuffer.h"

CircularBuffer::CircularBuffer(int minBuffer, int maxBuffer) : QObject(NULL),
    _minBuffer(minBuffer),
    _maxBuffer(maxBuffer),
    _maxTailleBuffer(2 * maxBuffer),
    _bufferSize(4 * maxBuffer),
    _posEcriture(0),
    _posLecture(0),
    _currentLengthAvailable(0),
    _interrupted(false),
    _isFinished(false)
{
    // Initialisation des buffers
    _dataL       = new float [_bufferSize];
    _dataR       = new float [_bufferSize];
    _dataTmpL    = new float [_bufferSize];
    _dataTmpR    = new float [_bufferSize];
    _dataRevL    = new float [_bufferSize];
    _dataRevR    = new float [_bufferSize];
    _dataTmpRevL = new float [_bufferSize];
    _dataTmpRevR = new float [_bufferSize];
}

CircularBuffer::~CircularBuffer()
{
    delete [] _dataL;
    delete [] _dataR;
    delete [] _dataTmpL;
    delete [] _dataTmpR;
    delete [] _dataRevL;
    delete [] _dataRevR;
    delete [] _dataTmpRevL;
    delete [] _dataTmpRevR;
}

bool CircularBuffer::isInterrupted()
{
    _mutexInterrupt.lock();
    bool bRet = _interrupted;
    _mutexInterrupt.unlock();
    return bRet;
}

void CircularBuffer::stop()
{
    _mutexInterrupt.lock();
    _interrupted = true;
    _mutexInterrupt.unlock();
    _mutexSynchro.tryLock();
    _mutexSynchro.unlock();
}

bool CircularBuffer::isFinished()
{
    _mutexInterrupt.lock();
    bool bRet = _isFinished;
    _mutexInterrupt.unlock();
    return bRet;
}
