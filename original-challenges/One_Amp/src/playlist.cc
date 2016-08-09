/*
 * Copyright (c) 2016 Kaprica Security, Inc.
 *
 * Permission is hereby granted, cgc_free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "playlist.h"
#include "compare.h"
#include "print.h"

cgc_Playlist::cgc_Playlist()
{
    length_ = 0;
    list_size_ = 0;
    playlist_ = (cgc_tag_and_file *)NULL;
}

bool cgc_Playlist::cgc_AddSong(const cgc_tag_and_file *song)
{
    if (!song || length_ >= MAX_PLAYLIST_LENGTH)
        return false;

    if (!playlist_)
    {
        list_size_ = 16;
        playlist_ = new cgc_tag_and_file[list_size_];
    }
    if (length_ == list_size_)
    {
        list_size_ *= 2;
        cgc_tag_and_file *doubled_list = new cgc_tag_and_file[list_size_];
        cgc_memcpy(doubled_list, playlist_, sizeof(cgc_tag_and_file) * length_);
        delete[] playlist_;
        playlist_ = doubled_list;
    }
    playlist_[length_++] = *song;
    return true;
}

bool cgc_Playlist::cgc_RemoveSong(const unsigned int song_id, cgc_tag_and_file *removed_song)
{
    bool found_song = false;
    unsigned int i;
    for (i = 0; i < length_; i++)
    {
        if (playlist_[i].tag.id == song_id)
        {
            if (removed_song)
                *removed_song = playlist_[i];
            found_song = true;
            break;
        }
    }

    if (found_song)
    {
        if (i < length_ - 1)
            cgc_memmove(&playlist_[i], &playlist_[i+1], sizeof(cgc_tag_and_file) * (length_ - (i+1)));
        --length_;
    }
    return found_song;
}

cgc_tag_and_file *cgc_Playlist::cgc_GetSong(const unsigned int song_id)
{
    cgc_tag_and_file *found_song = (cgc_tag_and_file *)NULL;
    for (unsigned int i = 0; i < length_; i++)
    {
        if (playlist_[i].tag.id == song_id)
        {
            found_song = &playlist_[i];
            break;
        }
    }
    return found_song;
}

void cgc_Playlist::cgc_SortById()
{
    cgc_Sort(playlist_, length_, &cgc_CompareIds);
}

void cgc_Playlist::cgc_SortByTitle()
{
    cgc_Sort(playlist_, length_, &cgc_CompareTitles);
}

void cgc_Playlist::cgc_SortByArtistAndAlbum()
{
    cgc_Sort(playlist_, length_, &cgc_CompareArtists);
    unsigned int start_idx = 0;
    char *prev_item = (char *)NULL;
    char *cur_item = (char *)NULL;
    if (length_)
        prev_item = playlist_[0].tag.artist;

    for (unsigned int i = 0; i < length_; i++)
    {
        cur_item = playlist_[i].tag.artist;
        if (cgc_memcmp(prev_item, cur_item, sizeof(playlist_[i].tag.artist) != 0))
        {
            cgc_Sort(&playlist_[start_idx], i - start_idx, &cgc_CompareAlbums);
            start_idx = i;
            prev_item = cur_item;
        }
    }
    if (length_)
        cgc_Sort(&playlist_[start_idx], length_ - start_idx, &cgc_CompareAlbums);
}

void cgc_Playlist::cgc_SortByArtistAndTitle()
{
    cgc_Sort(playlist_, length_, &cgc_CompareArtists);
    unsigned int start_idx = 0;
    char *prev_item = (char *)NULL;
    char *cur_item = (char *)NULL;
    if (length_)
        prev_item = playlist_[0].tag.artist;

    for (unsigned int i = 0; i < length_; i++)
    {
        cur_item = playlist_[i].tag.artist;
        if (cgc_memcmp(prev_item, cur_item, sizeof(playlist_[i].tag.artist) != 0))
        {
            cgc_Sort(&playlist_[start_idx], i - start_idx, &cgc_CompareTitles);
            start_idx = i;
            prev_item = cur_item;
        }
    }
    if (length_)
        cgc_Sort(&playlist_[start_idx], length_ - start_idx, &cgc_CompareTitles);
}

void cgc_Playlist::cgc_SortByAlbum()
{
    cgc_Sort(playlist_, length_, &cgc_CompareAlbums);
}

void cgc_Playlist::cgc_SortByAlbumAndTitle()
{
    cgc_Sort(playlist_, length_, &cgc_CompareAlbums);
    unsigned int start_idx = 0;
    char *prev_item = (char *)NULL;
    char *cur_item = (char *)NULL;
    if (length_)
        prev_item = playlist_[0].tag.album;

    for (unsigned int i = 0; i < length_; i++)
    {
        cur_item = playlist_[i].tag.album;
        if (cgc_memcmp(prev_item, cur_item, sizeof(playlist_[i].tag.album) != 0))
        {
            cgc_Sort(&playlist_[start_idx], i - start_idx, &cgc_CompareTitles);
            start_idx = i;
            prev_item = cur_item;
        }
    }
    if (length_)
        cgc_Sort(&playlist_[start_idx], length_ - start_idx, &cgc_CompareTitles);
}


void cgc_Playlist::cgc_ListAllSongs()
{
    for (unsigned int i = 0; i < length_; i++)
    {
        cgc_printf("Song ID: " ESC "d" NL, playlist_[i].tag.id);
        cgc_printf("Title: ");
        PRINT_ARR_CHARS(playlist_[i].tag.title, sizeof(playlist_[i].tag.title));
        cgc_printf(NL "Artist: ");
        PRINT_ARR_CHARS(playlist_[i].tag.artist, sizeof(playlist_[i].tag.artist));
        cgc_printf(NL "Album: ");
        PRINT_ARR_CHARS(playlist_[i].tag.album, sizeof(playlist_[i].tag.album));
        cgc_printf(NL "Year: " ESC "d" NL, playlist_[i].tag.year);
        cgc_printf("Track #: " ESC "d" NL, playlist_[i].tag.track_number);
        cgc_printf("----------" NL);
    }
}

void cgc_Playlist::cgc_ClearPlaylist(bool delete_playlist)
{
    if (delete_playlist && playlist_)
        delete[] playlist_;

    playlist_ = (cgc_tag_and_file *)NULL;
    length_ = 0;
    list_size_ = 0;
}

unsigned int cgc_Playlist::cgc_length()
{
    return length_;
}

//Private Functions
void cgc_Playlist::cgc_Sort(cgc_tag_and_file *playlist, unsigned int cgc_length, int (*compare_fn)(cgc_tag_and_file *playlist1, cgc_tag_and_file *playlist2))
{
    cgc_tag_and_file *duped_list = new cgc_tag_and_file[cgc_length];
    cgc_SortHelper(playlist, duped_list, 0, cgc_length, compare_fn);
    delete[] duped_list;
}

void cgc_Playlist::cgc_SortHelper(cgc_tag_and_file *playlist, cgc_tag_and_file *duped_list, unsigned int start_idx, unsigned int end_idx,
                    int (*compare_fn)(cgc_tag_and_file *playlist1, cgc_tag_and_file *playlist2))
{
    if (end_idx - start_idx < 2)
        return;

    int mid_idx = (end_idx + start_idx) / 2;
    cgc_SortHelper(playlist, duped_list, start_idx, mid_idx, compare_fn);
    cgc_SortHelper(playlist, duped_list, mid_idx, end_idx, compare_fn);

    int left_idx = start_idx;
    int right_idx = mid_idx;

    for (unsigned int i = start_idx; i < end_idx; i++)
    {
        if (left_idx < mid_idx)
        {
            if (right_idx  >= end_idx || compare_fn(&playlist[left_idx], &playlist[right_idx]) <= 0)
                duped_list[i] = playlist[left_idx++];
            else
                duped_list[i] = playlist[right_idx++];
        }
        else
        {
            duped_list[i] = playlist[right_idx++];
        }
    }
    cgc_memcpy(&playlist[start_idx], &duped_list[start_idx], (end_idx - start_idx) * sizeof(playlist[0]));
}

