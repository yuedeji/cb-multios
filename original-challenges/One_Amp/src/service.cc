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

#include <cstdlib.h>
#include <cstring.h>
#include "playlist.h"
#include "tag_and_file.h"
#include "print.h"

#define MAX_USER_PLAYLISTS 10
#define SECRET_PAGE_SIZE 4096

unsigned int g_song_idx = 0;

bool cgc_RecvNewSong(cgc_tag_and_file *new_song)
{
    new_song->file = new cgc_MgcFile();
    if (new_song->file->cgc_ReadMgcFile(stdin))
    {
        cgc_fflush(stdout);
        if (cgc_fread(&new_song->tag, new_song->tag.cgc_header_size(), stdin) != new_song->tag.cgc_header_size())
        {
            delete new_song;
            return false;
        }
        new_song->tag.id = g_song_idx++;
        return true;
    }

    return false;
}

void cgc_PrintPlaylist(cgc_Playlist *playlist, unsigned int playlist_id)
{
    if (!playlist)
        return;

    cgc_printf("cgc_Playlist ID: " ESC "d" NL, playlist_id);
    cgc_printf("Number of songs added to playlist: " ESC "d" NL, playlist->cgc_length());
    cgc_fflush(stdout);
}

extern "C" int __attribute__((fastcall)) main(int secret_page_i, char *unused[])
{
    unsigned char *secret_page = (unsigned char *)secret_page_i;
    /* If you want to obfuscate input/output, uncomment and change */
    cgc_fxlat(stdin, "393748225");
    cgc_fxlat(stdout, "393748225");

    unsigned int remix_idx;
    remix_idx = secret_page[2];
    cgc_fbuffered(stdout, 1);

    cgc_printf("--One Amp--" NL);
    cgc_printf(NL NL);
    cgc_fflush(stdout);

    cgc_Playlist *master = new cgc_Playlist();
    cgc_Playlist *selected_playlist = master;
    cgc_Playlist *user_playlists = new cgc_Playlist[MAX_USER_PLAYLISTS];
    unsigned int num_playlists = 0;

    char input[1024];
    int exit_program = false;
    int choice = 0;
    while(!exit_program)
    {
        cgc_printf("Main Menu:" NL);
        if (selected_playlist == master)
        {
            cgc_printf("1. Add Song To Library" NL);
            cgc_printf("2. Delete Song From Library" NL);
            cgc_printf("3. Select cgc_Playlist" NL);
        }
        else
        {
            cgc_printf("1. Add Song To cgc_Playlist" NL);
            cgc_printf("2. Delete Song From cgc_Playlist" NL);
            cgc_printf("3. Return to Main Library" NL);
        }
        cgc_printf("4. List Songs" NL);
        cgc_printf("5. cgc_Sort By Song ID (Default cgc_Sort)" NL);
        cgc_printf("6. cgc_Sort By Song Title" NL);
        cgc_printf("7. cgc_Sort By Artist and Album" NL);
        cgc_printf("8. cgc_Sort By Artist and Song Title" NL);
        cgc_printf("9. cgc_Sort By Album" NL);
        cgc_printf("10. cgc_Sort By Album and Song Title" NL);
        cgc_printf("11. cgc_Remix Track" NL);
        if (selected_playlist == master)
        {
            cgc_printf("12. Create cgc_Playlist" NL);
            cgc_printf("13. Delete cgc_Playlist" NL);
            cgc_printf("14. List Playlists" NL);
            cgc_printf("15. Exit" NL);
        }
        else
        {
            cgc_printf("12. List All Songs In Library" NL);
            cgc_printf("13. Exit" NL);
        }

        cgc_printf("[::]  ");
        cgc_fflush(stdout);

        if (cgc_freaduntil(input, sizeof(input), *NL, stdin) < 0)
            break;
        if (*input == 0)
            continue;
        choice = cgc_strtol(input, NULL, 10);
        if (choice == 1 && selected_playlist == master)
        {
            cgc_tag_and_file new_song;
            if (cgc_RecvNewSong(&new_song) && master->cgc_AddSong(&new_song))
                cgc_printf("Added song to library" NL);
            else
                cgc_printf("Could not add song to library" NL);
            cgc_fflush(stdout);
        }
        else if (choice == 2 && selected_playlist == master)
        {
            cgc_tag_and_file song_to_remove;
            cgc_printf("Enter Song ID to delete from library" NL);
            cgc_printf("[::] ");
            cgc_fflush(stdout);

            if (cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                continue;
            unsigned int song_id = cgc_strtol(input, NULL, 10);
            if (master->cgc_RemoveSong(song_id, &song_to_remove))
            {
                for (unsigned int i = 0; i < num_playlists; i++)
                    user_playlists[i].cgc_RemoveSong(song_id, (cgc_tag_and_file *)NULL);

                delete song_to_remove.file;
                cgc_printf("Successfully removed song from library" NL);
            }
            else
            {
                cgc_printf("Could not remove song from library" NL);
            }
            cgc_fflush(stdout);

        }
        else if (choice == 3 && selected_playlist == master)
        {
            cgc_printf("Select cgc_Playlist ID" NL);
            cgc_printf("[::] ");
            cgc_fflush(stdout);

            if (cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                continue;
            unsigned int playlist_id = cgc_strtol(input, NULL, 10);
            if(playlist_id < num_playlists)
                selected_playlist = &user_playlists[playlist_id];
            else
                cgc_printf("Bad cgc_Playlist ID" NL);
            cgc_fflush(stdout);
        }
        else if (choice == 1)
        {
            cgc_printf("Enter Song ID to add to playlist" NL);
            cgc_printf("[::] ");
            cgc_fflush(stdout);

            if (cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                continue;
            unsigned int song_id = cgc_strtol(input, NULL, 10);
            cgc_tag_and_file *playlist_song = master->cgc_GetSong(song_id);
            cgc_tag_and_file *song_already_added = selected_playlist->cgc_GetSong(song_id);
            if (playlist_song && !song_already_added && selected_playlist->cgc_AddSong(playlist_song))
                cgc_printf("Added song to playlist" NL);
            else
                cgc_printf("Could not add song to playlist" NL);
            cgc_fflush(stdout);
        }
        else if (choice == 2)
        {
            cgc_printf("Enter Song ID to delete from playlist" NL);
            cgc_printf("[::] ");
            cgc_fflush(stdout);

            if (cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                continue;
            unsigned int song_id = cgc_strtol(input, NULL, 10);
            if (selected_playlist->cgc_RemoveSong(song_id, (cgc_tag_and_file *)NULL))
                cgc_printf("Successfully removed song from playlist" NL);
            else
                cgc_printf("Could not remove song from playlist" NL);
            cgc_fflush(stdout);

        }
        else if (choice == 3)
        {
            selected_playlist = master;
        }
        else if (choice == 4)
        {
            selected_playlist->cgc_ListAllSongs();
        }
        else if (choice == 5)
        {
            selected_playlist->cgc_SortById();
            cgc_printf("Successfully sorted list by Song ID" NL);
            cgc_fflush(stdout);
        }
        else if (choice == 6)
        {
            selected_playlist->cgc_SortByTitle();
            cgc_printf("Successfully sorted list by Song Title" NL);
            cgc_fflush(stdout);
        }
        else if (choice == 7)
        {
            selected_playlist->cgc_SortByArtistAndAlbum();
            cgc_printf("Successfully sorted list by Artist Name and Album" NL);
            cgc_fflush(stdout);
        }
        else if (choice == 8)
        {
            selected_playlist->cgc_SortByArtistAndTitle();
            cgc_printf("Successfully sorted list by Artist Name and Song Title" NL);
            cgc_fflush(stdout);
        }
        else if (choice == 9)
        {
            selected_playlist->cgc_SortByAlbum();
            cgc_printf("Successfully sorted list by Album" NL);
            cgc_fflush(stdout);
        }
        else if (choice == 10)
        {
            selected_playlist->cgc_SortByAlbumAndTitle();
            cgc_printf("Successfully sorted list by Album and Song Title" NL);
            cgc_fflush(stdout);
        }
        else if (choice == 11)
        {
            cgc_printf("Enter Song ID to remix" NL);
            cgc_printf("[::] ");
            cgc_fflush(stdout);

            if (cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                continue;
            unsigned int song_id = cgc_strtol(input, NULL, 10);
            cgc_tag_and_file *song = selected_playlist->cgc_GetSong(song_id);
            if (song)
            {
                cgc_printf("Original Track Data:" NL);
                song->file->cgc_PrintFrameData();
                unsigned int secret_page_size = SECRET_PAGE_SIZE;
                song->file->cgc_Remix(secret_page, &remix_idx, secret_page_size);
                cgc_printf("Successfully remixed song" NL);
                cgc_printf("Printing New Track Data:" NL);
                song->file->cgc_PrintFrameData();
            }
            else
            {
                cgc_printf("Could not remix track" NL);
            }
            cgc_fflush(stdout);
        }
        else if (choice == 12 && selected_playlist == master)
        {
            if(num_playlists < MAX_USER_PLAYLISTS)
            {
                user_playlists[num_playlists].cgc_ClearPlaylist(true);
                cgc_printf("Created a new playlist with ID [" ESC "d]" NL, num_playlists++);
            }
            else
            {
                cgc_printf("Maximum number of playlists reached" NL);
            }
            cgc_fflush(stdout);
        }
        else if (choice == 13 && selected_playlist == master)
        {
            cgc_printf("Enter cgc_Playlist ID to delete");
            cgc_printf("[::] ");
            cgc_fflush(stdout);

            if (cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                continue;
            unsigned int playlist_id = cgc_strtol(input, NULL, 10);
            if(playlist_id < num_playlists)
            {
                user_playlists[playlist_id].cgc_ClearPlaylist(true);
                if (playlist_id < num_playlists - 1)
                {
                    cgc_memmove(&user_playlists[playlist_id], &user_playlists[playlist_id+1],
                            sizeof(user_playlists[0]) * (num_playlists - (playlist_id+1)));
                }
                --num_playlists;
                user_playlists[num_playlists].cgc_ClearPlaylist(false);
                cgc_printf("Deleted cgc_Playlist ID: " ESC "d" NL, playlist_id);
            }
            else
            {
                cgc_printf("Bad cgc_Playlist ID" NL);
            }
            cgc_fflush(stdout);
        }
        else if (choice == 14 && selected_playlist == master)
        {
            for (unsigned int i = 0; i < num_playlists; i++)
                cgc_PrintPlaylist(&user_playlists[i], i);
        }
        else if (choice == 15 && selected_playlist == master)
        {
            exit_program = true;
        }
        else if (choice == 12)
        {
            master->cgc_ListAllSongs();
        }
        else if (choice == 13)
        {
            exit_program = true;
        }
    }

    cgc_printf("--Exited One Amp--" NL);
    cgc_fflush(stdout);

    return 0;
}
