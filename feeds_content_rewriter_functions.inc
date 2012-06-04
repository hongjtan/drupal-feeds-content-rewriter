<?php
/*
 * To do:
 */
      
      // Function to convert a string to a multibyte string given an array of optional stopwords and optional ignored words.
      function mb_string_convert ($string, $stopwords = NULL, $ignored_words = NULL)
      { 
        if(!function_exists('mb_char_convert'))
        {
          // Nested function to convert each individual character of a string to a multibyte string.
          function mb_char_convert($string)
          {
            $new_multibyte_string = "";
            $special_characters = array(
                'a' => 'а',
                'c' => 'с',
                'e' => 'е',
                'i' => 'і',
                'o' => 'о',
                's' => 'ѕ',
                'y' => 'у'
                );

            $char_counter = 0;
            $multibyte_char_counter = 0;

            while($char_counter < strlen($string))
            {
                // If the letter matches one of the keys in the special character
                // array, randomize a number. If <= 75, use special letter, else
                // use the original character.
                if (array_key_exists($string[$char_counter], $special_characters))
                {
                    if(mt_rand(1, 100) <= 75)
                    {
                        $new_multibyte_string[$multibyte_char_counter] = $special_characters[$string[$char_counter]];
                        $multibyte_char_counter += strlen($special_characters[$string[$char_counter]]);
                    } // end if
                    else 
                    {
                        $new_multibyte_string[$multibyte_char_counter] = $string[$char_counter];
                        $multibyte_char_counter += strlen($string[$char_counter]);
                    } // end else
                } // end if
                // If nothing matches, then it doesn't have a special character,
                // so put the original character inside of the string.
                else
                {
                    $new_multibyte_string[$multibyte_char_counter] = $string[$char_counter];
                    $multibyte_char_counter += strlen($string[$char_counter]);
                } // end else

                $char_counter++;
            } // end while

            return implode('', $new_multibyte_string);
          } // end function
        }// end if
        
        // Convert a string to multibyte given no stopwords or ignored words.
        if($stopwords == NULL && $ignored_words == NULL)
        {
          //echo $stopwords;
          return mb_char_convert($string);
        }

        // If there are stopwords, only convert stopwords and ignore ignored words.
        else if($stopwords != NULL)
        {
          $converted_string = $string;
          $converted_string_array = explode(' ', $converted_string);
          $word_counter = 0;
          
          foreach ($converted_string_array as $converted_word)
          {
              if (is_array($ignored_words) && !empty($ignored_words))
              {
                  foreach($ignored_words as $ignored_word)
                  {   
                      // Increase the word counter to the next word and skip the current word if the word
                      // is an ignored word.
                      if(strcasecmp(trim($ignored_word), trim($converted_word)) == 0)
                      {
                          $word_counter++;
                          continue 2;
                      } // end if
                  } // end foreach
              } // end if
              
              if (in_array($converted_word, array_map('trim', $stopwords)))
                $converted_string_array[$word_counter] = mb_char_convert($converted_word);
          
              $word_counter++;
          } // end foreach
          
          $converted_string = implode(' ', $converted_string_array);
          
          return $converted_string;
        } // end else if
        
        // If there are no stopwords but ignored words are set, only process ignored words.
        else if($stopwords == NULL && $ignored_words != NULL)
        {
          $converted_string = $string;
          $converted_string_array = explode(' ', $converted_string);
          $word_counter = 0;

          foreach ($converted_string_array as $converted_word)
          {
              if (is_array($ignored_words) && !empty($ignored_words))
              {
                  foreach($ignored_words as $ignored_word)
                  {   
                      // Increase the word counter to the next word and skip the current word if the word
                      // is an ignored word.
                      if(strcasecmp(trim($ignored_word), trim($converted_word)) == 0)
                      {
                          $word_counter++;
                          continue 2;
                      } // end if
                  } // end foreach
              } // end if

              if (!empty($converted_word))
                  $converted_string_array[$word_counter] = mb_char_convert($converted_word);
              
              $word_counter++;
          } // end foreach
          
          $converted_string = implode(' ', $converted_string_array);
          
          return $converted_string;
        } // end else if
      } // end function
      
      // Get all possible spun strings given an input string, returns an array of all the possible strings.
      function spin_string($string_to_spin, $number_to_spin = 0)
      {
        if (!preg_match ('/\{(.*?)\}/', $string_to_spin))
          return 0;

        $current_position = 0;
        $spun_strings = array();

        while (preg_match ('/\{(.*?)\}/', $string_to_spin, $spin_match, PREG_OFFSET_CAPTURE, $current_position))
        {
          $current_position = $spin_match[1][1];        // The current position of the search.
          $spin_words = explode('|', $spin_match[1][0]); // The matched spin words put into an array.

          // Fill the first strings into the $spun_strings array.
          if(empty($spun_strings))
          {
            foreach($spin_words as $spin_word)
            {
              array_push($spun_strings, preg_replace('/'.preg_quote($spin_match[0][0]).'/', $spin_word, $string_to_spin, 1));
            }
          }

          // Subsequent iterations should create a new array, go through the old array and treat each of the spun strings as
          // their own separate string to spin until all of them are parsed.
          else
          {
            $new_spun_strings = array();

            foreach($spun_strings as $spun_string)
            {
              foreach($spin_words as $spin_word)
              {
                array_push($new_spun_strings, preg_replace('/'.preg_quote($spin_match[0][0]).'/', $spin_word, $spun_string, 1));
              }
            }

            $spun_strings = $new_spun_strings;
          }
        }

        if((!is_numeric($number_to_spin) && $number_to_spin != 0)) {
            return 0;
        }
        else if(is_numeric($number_to_spin) && $number_to_spin > sizeof($spun_strings)) {
            return $spun_strings;
        }
        else if(is_numeric($number_to_spin) && $number_to_spin != 0 && $number_to_spin <= sizeof($spun_strings)) {
            srand(time());
            $random_number_array = array();

            for($current_position=0; $current_position<$number_to_spin; $current_position++) {
                $random_number = mt_rand(0, sizeof($spun_strings) - 1);

                while(in_array($random_number, $random_number_array)) {
                    $random_number = mt_rand(0, sizeof($spun_strings) - 1);
                }

                array_push($random_number_array, $random_number);
            }

            $random_spun_strings = array ();

            foreach($random_number_array as $random_number) {
                array_push($random_spun_strings, $spun_strings[$random_number]);
            }

            return $random_spun_strings;
        }
        else if(is_numeric($number_to_spin) && $number_to_spin == 0)
            return $spun_strings;
      }
      
      // Function to print multibyte characters strong given an input string.
      function mb_string_print($string) {
        // Split the entire paragraph into individual characters and
        // every character more than 1 byte should be bolded.
        $split_string = preg_split('//u', nl2br ($string), -1, PREG_SPLIT_NO_EMPTY);
                
        foreach ($split_string as $character) {
          if(strlen($character) > 1)
            echo '<strong>' . $character . '</strong>';
          else
            echo $character;
        }
        
        return 1;
      }
      
      // Function to create a string with all the multibyte characters with a strong HTML tag.
      function mb_string_strong($string) {
          // Split the entire paragraph into individual characters and
          // every character more than 1 byte should be bolded.
          $split_string = preg_split('//u', nl2br ($string), -1, PREG_SPLIT_NO_EMPTY);
          $new_string = array();
          
          foreach ($split_string as $character) {
              if(strlen($character) > 1)
                  array_push($new_string, '<strong>' . $character . '</strong>');
              else
                  array_push($new_string, $character);
          }
              
          return implode('', $new_string);
      }