
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2010 Francois Beaune
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

// Interface header.
#include "tile.h"

// appleseed.foundation headers.
#include "foundation/core/exceptionnotimplemented.h"

namespace foundation
{

//
// Tile class implementation.
//

// Construct a new tile.
Tile::Tile(
    const size_t        width,
    const size_t        height,
    const size_t        channel_count,
    const PixelFormat   pixel_format,
    uint8*              storage)
  : m_width(width)
  , m_height(height)
  , m_channel_count(channel_count)
  , m_pixel_format(pixel_format)
{
    assert(m_width > 0);
    assert(m_height > 0);
    assert(m_channel_count > 0);

    // Compute the total number of pixels.
    m_pixel_count = m_width * m_height;

    // Compute the size in bytes of one channel.
    m_channel_size = Pixel::size(m_pixel_format);

    // Compute the size in bytes of one pixel.
    m_pixel_size = m_channel_size * m_channel_count;

    // Compute the size in bytes of the pixel array.
    m_array_size = m_pixel_size * m_pixel_count;

    // Allocate the pixel array.
    if (storage)
    {
         m_pixel_array = storage;
         m_own_storage = false;
    }
    else
    {
        m_pixel_array = new uint8[m_array_size];
        m_own_storage = true;
    }
}

// Construct a tile by converting an existing tile to a given pixel format.
Tile::Tile(
    const Tile&         tile,
    const PixelFormat   pixel_format,
    uint8*              storage)
  : m_width(tile.m_width)
  , m_height(tile.m_height)
  , m_channel_count(tile.m_channel_count)
  , m_pixel_format(pixel_format)
  , m_pixel_count(tile.m_pixel_count)
{
    // Compute the size in bytes of one channel.
    m_channel_size = Pixel::size(m_pixel_format);

    // Compute the size in bytes of one pixel.
    m_pixel_size = m_channel_size * m_channel_count;

    // Compute the size in bytes of the pixel array.
    m_array_size = m_pixel_size * m_pixel_count;

    // Allocate pixel array.
    if (storage)
    {
         m_pixel_array = storage;
         m_own_storage = false;
    }
    else
    {
        m_pixel_array = new uint8[m_array_size];
        m_own_storage = true;
    }

    // Convert pixels.
    Pixel::convert(
        tile.m_pixel_format,                        // source format
        tile.m_pixel_array,                         // source begin
        tile.m_pixel_array + tile.m_array_size,     // source end
        1,                                          // source stride
        m_pixel_format,                             // destination format
        m_pixel_array,                              // destination
        1);                                         // destination stride
}

// Construct a tile by converting an existing tile to a given pixel format,
// and allowing reordering, replication and deletion of channels.
Tile::Tile(
    const Tile&         tile,
    const PixelFormat   pixel_format,
    const size_t*       shuffle_table,
    uint8*              storage)
  : m_width(tile.m_width)
  , m_height(tile.m_height)
  , m_pixel_format(pixel_format)
  , m_pixel_count(tile.m_pixel_count)
{
    // Compute the new number of channels.
    m_channel_count =
        Pixel::get_dest_channel_count(
            tile.m_channel_count,
            shuffle_table);

    // Compute the size in bytes of one channel.
    m_channel_size = Pixel::size(m_pixel_format);

    // Compute the size in bytes of one pixel.
    m_pixel_size = m_channel_size * m_channel_count;

    // Compute the size in bytes of the pixel array.
    m_array_size = m_pixel_size * m_pixel_count;

    // Allocate pixel array.
    if (storage)
    {
         m_pixel_array = storage;
         m_own_storage = false;
    }
    else
    {
        m_pixel_array = new uint8[m_array_size];
        m_own_storage = true;
    }

    // Convert pixels.
    Pixel::convert_and_shuffle(
        tile.m_pixel_format,                        // source format
        tile.m_channel_count,                       // source channels
        tile.m_pixel_array,                         // source begin
        tile.m_pixel_array + tile.m_array_size,     // source end
        m_pixel_format,                             // destination format
        m_channel_count,                            // destination channels
        m_pixel_array,                              // destination
        shuffle_table);                             // channel shuffling table
}

// Copy constructor.
Tile::Tile(const Tile&  rhs)
  : m_width(rhs.m_width)
  , m_height(rhs.m_height)
  , m_channel_count(rhs.m_channel_count)
  , m_pixel_format(rhs.m_pixel_format)
  , m_pixel_count(rhs.m_pixel_count)
  , m_channel_size(rhs.m_channel_size)
  , m_pixel_size(rhs.m_pixel_size)
  , m_array_size(rhs.m_array_size)
{
    // Allocate pixel array.
    m_pixel_array = new uint8[m_array_size];
    m_own_storage = true;

    // Copy pixels from source tile.
    std::memcpy(m_pixel_array, rhs.m_pixel_array, m_array_size);
}

// Destructor.
Tile::~Tile()
{
    // Deallocate pixel array.
    if (m_own_storage)
        delete [] m_pixel_array;
}

// Implements the foundation::ISerializable interface.
Serializer* Tile::serialize(Serializer* serializer)
{
    throw ExceptionNotImplemented();
    return serializer;
}
Deserializer* Tile::deserialize(Deserializer* deserializer)
{
    throw ExceptionNotImplemented();
    return deserializer;
}

// Return the size of a tile, including the dynamically allocated memory.
size_t dynamic_sizeof(const Tile& tile)
{
    return sizeof(Tile) + tile.get_size();
}

}   // namespace foundation
