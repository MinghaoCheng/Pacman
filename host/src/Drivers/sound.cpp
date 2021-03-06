/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#include "sound.h"

char string[100];
/**
 * Reconstruct the string
 * 
 * @param string
 * @return None
 */
void sys_printf(char *fmt,...)
{
    va_list ap;
    va_start(ap, fmt);
    vsprintf(string, fmt, ap);
    system(string);
    va_end(ap);
}

/**
 * Constructor, pass the Thread ID
 * 
 * @param ID of the thread
 * @return None
 */
Sound_dev::Sound_dev(pthread_t ID):Thread(ID)
{
    
}

/**
 * Destructor
 * 
 * @param None
 * @return None
 */
Sound_dev::~Sound_dev()
{
    
}

/**
 * Initialise
 * 
 * @param None
 * @return result, 0 --> success
 */
int8_t Sound_dev::init(void)
{
    this->flag = false;
    return 0;
}

/**
 * Pass the file name to global string and play it in another thread
 * 
 * @param pointer to the file
 * @return None
 */
void Sound_dev::play_wav(const char *File_name)
{
    this->file_name = (char*)File_name;
    if(!this->flag)
    {
        this->flag = true;
    }
}

/**
 * Play the wav file in the current thread
 * 
 * @param pointer to the file
 * @return None
 */
void Sound_dev::play_wav_blocking(const char *File_name)
{
    sys_printf((char *)"aplay %s\n", File_name);
}

/**
 * Thread function, play the sound file
 * 
 * @param None
 * @return None
 */
void Sound_dev::thread_handler(void)
{
    while(1)
    {
        if(this->flag)
        {
            this->flag = false;
            sys_printf((char *)"aplay %s\n", this->file_name);
        }
        usleep(1000);
    }
}

/*
#include <alsa/asoundlib.h>

unsigned char metadata[44];
void check_info(const char* name)
{
    int val;
    printf("ALSA library version: %s\n", SND_LIB_VERSION_STR);
    printf("\nPCM stream types:\n");
    for (val = 0; val <= SND_PCM_STREAM_LAST; val++)
    {
        printf("  %s\n", snd_pcm_stream_name((snd_pcm_stream_t)val));
    }
    printf("\nPCM access types:\n");
    for (val = 0; val <= SND_PCM_ACCESS_LAST; val++)
    {
      printf("  %s\n", snd_pcm_access_name((snd_pcm_access_t)val));
    }

    printf("\nPCM formats:\n");
    for (val = 0; val <= SND_PCM_FORMAT_LAST; val++)
    {
        if (snd_pcm_format_name((snd_pcm_format_t)val)!= NULL)
        {
            printf("  %s (%s)\n", snd_pcm_format_name((snd_pcm_format_t)val), snd_pcm_format_description((snd_pcm_format_t)val));
        }
    }

    printf("\nPCM subformats:\n");
    for (val = 0; val <= SND_PCM_SUBFORMAT_LAST; val++)
    {
        printf("  %s (%s)\n", snd_pcm_subformat_name((snd_pcm_subformat_t)val), snd_pcm_subformat_description((snd_pcm_subformat_t)val));
    }
    
    printf("\nPCM states:\n");
    for (val = 0; val <= SND_PCM_STATE_LAST; val++)
    {
        printf("  %s\n", snd_pcm_state_name((snd_pcm_state_t)val));
    }
}
*/
