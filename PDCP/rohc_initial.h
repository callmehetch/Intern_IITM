
    /*
      Date  : 23-05-2019
      Authors : Viknesh, Ganesh, Nanda
      5G Testbed summer interns
    */

    #include <rohc/rohc.h>
 
    #include <stdio.h>  /* for the printf() function */
    #include <string.h>
    #include <stdlib.h>
     
    /* includes required to create a fake IP packet */
    #include <netinet/ip.h>  /* for the IPv4 header */
    #include <string.h>      /* for the strlen() */
    #include <rohc/rohc_buf.h>  /* for the rohc_buf_*() functions */
     
    /* includes required to use the compression part of the ROHC library */
    #include <time.h>             /* required by time() */
    #include <rohc/rohc_comp.h>   /* for rohc_comp_*() functions */
    #include <rohc/rohc_decomp.h> /* for rohc_decomp_*() functions */
 
    /* The size (in bytes) of the buffers used in the program */
    #define BUFFER_SIZE 2048
     
    /* The payload for the fake IP packet */
    // #define FAKE_PAYLOAD "hello, ROHC world!"
     
    /* return a random number every time it is called */
    static int gen_random_num(const struct rohc_comp *const comp,
                              void *const user_context)
    {
       return rand();
    }

    char str[BUFFER_SIZE];
    
    int ROHC(char *s);
    int ROHD(char *s);

    int ROHC(char *s)
    {
       struct rohc_comp *compressor;  /* the ROHC compressor */
       uint8_t ip_buffer[BUFFER_SIZE]; /* the buffer that will contain the IPv4 packet to compress */
       struct rohc_buf ip_packet = rohc_buf_init_empty(ip_buffer, BUFFER_SIZE); /* the packet that will contain the IPv4 packet to compress */
       struct iphdr *ip_header; /* the header of the IPv4 packet */
       uint8_t rohc_buffer[BUFFER_SIZE]; /* the packet that will contain the resulting ROHC packet */
       struct rohc_buf rohc_packet = rohc_buf_init_empty(rohc_buffer, BUFFER_SIZE); //..
       rohc_status_t rohc_status;
       size_t i;
     
       printf("This program will compress one single IPv4 packet\n");
     
       srand(time(NULL)); /* initialize the random generator with the current time */
     
       printf("create the ROHC compressor\n");
       compressor = rohc_comp_new2(ROHC_SMALL_CID, ROHC_SMALL_CID_MAX, gen_random_num, NULL);
       
       if(compressor == NULL)
       {
          fprintf(stderr, "failed create the ROHC compressor\n"); /* leave with an error code */
          return 1;
       }
     
       /* enable the IP-only compression profile */
       printf("enable the IP-only compression profile\n");
       if(!rohc_comp_enable_profile(compressor, ROHC_PROFILE_IP))
       {
          fprintf(stderr, "failed to enable the IP-only profile\n"); /* cleanup compressor, then leave with an error code */
          rohc_comp_free(compressor);
          return 1;
       }
     
       /*This will actually be in the packets*/
       printf("build a fake IP packet\n"); /* create a fake IP packet for the purpose of this simple program */
       ip_header = (struct iphdr *) rohc_buf_data(ip_packet);
       ip_header->version = 4; /* we create an IP header version 4 */
       ip_header->ihl = 5; /* min. IPv4 header length (in 32-bit words) */
       ip_packet.len += ip_header->ihl * 4;
       ip_header->tos = 0; /* TOS is not important for the example */
       ip_header->tot_len = htons(ip_packet.len + strlen(s));
       ip_header->id = 0; /* ID is not important for the example */
       ip_header->frag_off = 0; /* No packet fragmentation */
       ip_header->ttl = 1; /* TTL is not important for the example */
       ip_header->protocol = 134; /* protocol number */
       ip_header->check = 0x3fa9; /* checksum */
       ip_header->saddr = htonl(0x01020304); /* source address 1.2.3.4 */
       ip_header->daddr = htonl(0x05060708); /* destination addr. 5.6.7.8 */
     
       rohc_buf_append(&ip_packet, (uint8_t *) s, strlen(s)); /* copy the payload just after the IP header */
       
       for(i = 0; i < ip_packet.len; i++) /* dump the newly-created IP packet on terminal */
       {
          printf("0x%02x ", rohc_buf_byte_at(ip_packet, i));
          if(i != 0 && ((i + 1) % 8) == 0)
             printf("\n");
       }
       if(i != 0 && (i % 8) != 0) /* be sure to go to the line */
          printf("\n");
     
       printf("compress the fake IP packet\n"); /* compress the fake IP packet */
       rohc_status = rohc_compress4(compressor, ip_packet, &rohc_packet);
       if(rohc_status != ROHC_STATUS_OK)
       {
          fprintf(stderr, "compression of fake IP packet failed: %s (%d)\n", rohc_strerror(rohc_status), rohc_status); /* cleanup compressor, then leave with an error code */
          rohc_comp_free(compressor);
          return 1;
       }
     
       printf("ROHC packet resulting from the ROHC compression:\n"); /* dump the ROHC packet on terminal */
       for(i = 0; i < rohc_packet.len; i++)
       {
          printf("0x%02x ", rohc_buf_byte_at(rohc_packet, i));
          if(i != 0 && ((i + 1) % 8) == 0)
             printf("\n");
       }

       s = rohc_packet.data;

       if(i != 0 && (i % 8) != 0) /* be sure to go to the line */
          printf("\n");
       

       printf("destroy the ROHC compressor\n");
       rohc_comp_free(compressor);
     
       // ROHD(str);

       /* leave the program with a success code */
       return 0;
    }

    int ROHD(char *s)
    {
    //! [define ROHC decompressor]
      struct rohc_decomp *decompressor;       /* the ROHC decompressor */
    //! [define ROHC decompressor]

    //! [define IP and ROHC packets]
      /* the buffer that will contain the ROHC packet to decompress */
      unsigned char rohc_buffer[BUFFER_SIZE];
      struct rohc_buf rohc_packet = rohc_buf_init_empty(rohc_buffer, BUFFER_SIZE);
      /* the buffer that will contain the resulting IP packet */
      unsigned char ip_buffer[BUFFER_SIZE];
      struct rohc_buf ip_packet = rohc_buf_init_empty(ip_buffer, BUFFER_SIZE);
      /* we do not want to handle feedback in this simple example */
      struct rohc_buf *rcvd_feedback = NULL;
      struct rohc_buf *feedback_send = NULL;
    //! [define IP and ROHC packets]

      rohc_status_t status;
      size_t i;

    //! [create ROHC decompressor #1]
      /* Create a ROHC decompressor to operate:
       *  - with large CIDs,
       *  - with the maximum of 5 streams (MAX_CID = 4),
       *  - in Unidirectional mode (U-mode).
       */
    //! [create ROHC decompressor #1]
      printf("\ncreate the ROHC decompressor\n");
    //! [create ROHC decompressor #2]
      decompressor = rohc_decomp_new2(ROHC_LARGE_CID, 4, ROHC_U_MODE);
      if(decompressor == NULL)
      {
        fprintf(stderr, "failed create the ROHC decompressor\n");
        goto error;
      }
    //! [create ROHC decompressor #2]

      /* Enable the decompression profiles you need */
      printf("\nenable several ROHC decompression profiles\n");
    //! [enable ROHC decompression profile]
      if(!rohc_decomp_enable_profile(decompressor, ROHC_PROFILE_UNCOMPRESSED))
      {
        fprintf(stderr, "failed to enable the Uncompressed profile\n");
        goto release_decompressor;
      }
      if(!rohc_decomp_enable_profile(decompressor, ROHC_PROFILE_IP))
      {
        fprintf(stderr, "failed to enable the IP-only profile\n");
        goto release_decompressor;
      }
    //! [enable ROHC decompression profile]
    //! [enable ROHC decompression profiles]
      if(!rohc_decomp_enable_profiles(decompressor, ROHC_PROFILE_UDP,
                                      ROHC_PROFILE_UDPLITE, -1))
      {
        fprintf(stderr, "failed to enable the IP/UDP and IP/UDP-Lite "
                "profiles\n");
        goto release_decompressor;
      }
    //! [enable ROHC decompression profiles]


      printf("\nbuild a fake ROHC packet\n");
      rohc_packet.len = 0;

      for (int j = 0; j < strlen(s); ++j)
      {
        rohc_buf_byte_at(rohc_packet, rohc_packet.len++) = s[i];
      }

      /* dump the newly-created ROHC packet on terminal */
      for(i = 0; i < rohc_packet.len; i++)
      {
        printf("0x%02x ", rohc_buf_byte_at(rohc_packet,i));
        if(i != 0 && ((i + 1) % 8) == 0)
        {
          printf("\n");
        }
      }
      if(i != 0 && ((i + 1) % 8) != 0) /* be sure to go to the line */
      {
        printf("\n");
      }


      /* Now, decompress this fake ROHC packet */
      printf("\ndecompress the fake ROHC packet\n");
    //! [decompress ROHC packet #1]
      status = rohc_decompress3(decompressor, rohc_packet, &ip_packet,
                                rcvd_feedback, feedback_send);
    //! [decompress ROHC packet #1]
      printf("\n");
    //! [decompress ROHC packet #2]
      if(status == ROHC_STATUS_OK)
      {
        /* decompression is successful */
        if(!rohc_buf_is_empty(ip_packet))
        {
          /* ip_packet.len bytes of decompressed IP data available in
           * ip_packet: dump the IP packet on the standard output */
          printf("IP packet resulting from the ROHC decompression:\n");
          for(i = 0; i < ip_packet.len; i++)
          {
            printf("0x%02x ", rohc_buf_byte_at(ip_packet, i));
            if(i != 0 && ((i + 1) % 8) == 0)
            {
              printf("\n");
            }
            s[i] = rohc_buf_byte_at(ip_packet, i);
          }
          s[i] = '\0';
          if(i != 0 && ((i + 1) % 8) != 0) /* be sure to go to the line */
          {
            printf("\n");
          }
        }
        else
        {
          /* no IP packet was decompressed because of ROHC segmentation or
           * feedback-only packet:
           *  - the ROHC packet was a non-final segment, so at least another
           *    ROHC segment is required to be able to decompress the full
           *    ROHC packet
           *  - the ROHC packet was a feedback-only packet, it contained only
           *    feedback information, so there was nothing to decompress */
          printf("no IP packet decompressed");
        }
      }
      else
      {
        /* failure: decompressor failed to decompress the ROHC packet */
        fprintf(stderr, "decompression of fake ROHC packet failed\n");
    //! [decompress ROHC packet #2]
        goto release_decompressor;
    //! [decompress ROHC packet #3]
      }
    //! [decompress ROHC packet #3]


      /* Release the ROHC decompressor when you do not need it anymore */
      printf("\n\ndestroy the ROHC decompressor\n");
    //! [destroy ROHC decompressor]
      rohc_decomp_free(decompressor);
    //! [destroy ROHC decompressor]


      printf("\nThe program ended successfully.\n");

      return 0;

    release_decompressor:
      rohc_decomp_free(decompressor);
    error:
      fprintf(stderr, "an error occurred during program execution, "
              "abort program\n");
      return 1;
    }
