/*
ioapi.c -- IO base function header for compress/uncompress .zip
files using zlib + zip or unzip API

Version 1.01e, February 12th, 2005

Copyright (C) 1998-2010 Gilles Vollant
Copyright (C) 2009-2010 Mathias Svensson
Copyright (C) 2005-2011 Sergey A. Tachenov

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zlib.h"
#include "ioapi.h"
#include "quazip_global.h"
#include <QIODevice>


/* I've found an old Unix (a SunOS 4.1.3_U1) without all SEEK_* defined.... */

#ifndef SEEK_CUR
#define SEEK_CUR    1
#endif

#ifndef SEEK_END
#define SEEK_END    2
#endif

#ifndef SEEK_SET
#define SEEK_SET    0
#endif

voidpf ZCALLBACK qiodevice_open_file_func (
   voidpf /*opaque UNUSED*/,
   voidpf file,
   int mode)
{
    QIODevice *iodevice = reinterpret_cast<QIODevice*>(file);
    if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER)==ZLIB_FILEFUNC_MODE_READ)
        iodevice->open(QIODevice::ReadOnly);
    else
    if (mode & ZLIB_FILEFUNC_MODE_EXISTING)
        iodevice->open(QIODevice::ReadWrite);
    else
    if (mode & ZLIB_FILEFUNC_MODE_CREATE)
        iodevice->open(QIODevice::WriteOnly);

    if (iodevice->isOpen()) {
        if (iodevice->isSequential()) {
            iodevice->close();
            return NULL;
        } else {
            return iodevice;
        }
    } else
        return NULL;
}


uLong ZCALLBACK qiodevice_read_file_func (
   voidpf /*opaque UNUSED*/,
   voidpf stream,
   void* buf,
   uLong size)
{
    uLong ret;
    ret = (uLong)((QIODevice*)stream)->read((char*)buf,size);
    return ret;
}


uLong ZCALLBACK qiodevice_write_file_func (
   voidpf /*opaque UNUSED*/,
   voidpf stream,
   const void* buf,
   uLong size)
{
    uLong ret;
    ret = (uLong)((QIODevice*)stream)->write((char*)buf,size);
    return ret;
}

uLong ZCALLBACK qiodevice_tell_file_func (
   voidpf /*opaque UNUSED*/,
   voidpf stream)
{
    uLong ret;
    ret = ((QIODevice*)stream)->pos();
    return ret;
}

int ZCALLBACK qiodevice_seek_file_func (
   voidpf /*opaque UNUSED*/,
   voidpf stream,
   uLong offset,
   int origin)
{
    uLong qiodevice_seek_result=0;
    int ret;
    switch (origin)
    {
    case ZLIB_FILEFUNC_SEEK_CUR :
        qiodevice_seek_result = ((QIODevice*)stream)->pos() + offset;
        break;
    case ZLIB_FILEFUNC_SEEK_END :
        qiodevice_seek_result = ((QIODevice*)stream)->size() - offset;
        break;
    case ZLIB_FILEFUNC_SEEK_SET :
        qiodevice_seek_result = offset;
        break;
    default: return -1;
    }
    ret = !((QIODevice*)stream)->seek(qiodevice_seek_result);
    return ret;
}

int ZCALLBACK qiodevice_close_file_func (
   voidpf /*opaque UNUSED*/,
   voidpf stream)
{
    ((QIODevice*)stream)->close();
    return 0;
}

int ZCALLBACK qiodevice_error_file_func (
   voidpf /*opaque UNUSED*/,
   voidpf /*stream UNUSED*/)
{
    // can't check for error due to the QIODevice API limitation
    return 0;
}

void fill_qiodevice_filefunc (
  zlib_filefunc_def* pzlib_filefunc_def)
{
    pzlib_filefunc_def->zopen_file = qiodevice_open_file_func;
    pzlib_filefunc_def->zread_file = qiodevice_read_file_func;
    pzlib_filefunc_def->zwrite_file = qiodevice_write_file_func;
    pzlib_filefunc_def->ztell_file = qiodevice_tell_file_func;
    pzlib_filefunc_def->zseek_file = qiodevice_seek_file_func;
    pzlib_filefunc_def->zclose_file = qiodevice_close_file_func;
    pzlib_filefunc_def->zerror_file = qiodevice_error_file_func;
    pzlib_filefunc_def->opaque = NULL;
}