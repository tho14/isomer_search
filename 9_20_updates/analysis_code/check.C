if(isImplantIsomer)
  {
    // Get position info from the anode energy distribution fit
    xPos = static_cast<int>(bdecay.pspmt.lxpos);
    yPos = static_cast<int>(bdecay.pspmt.lypos);

    // Sanity check on position
    // Note that the 16x16 anode is 0 indexed on both axes!
    if(xPos >= 0 && xPos < npspmt)
      if(yPos >= 0 && yPos < npspmt)
	{
	  goodPos = true;
	  condition = 80;
	}
	  
    if(!goodPos)
      {
	condition = 84;
      }
    else
      { // We have a good position
	// Check existing implant
	// Check again!!!!
	//double timeDiffIon = -1;
	if(gImplant[xPos][yPos].time >= 0)
	  {
	    //timeDiffIon = std::abs(current_time - gImplant[xPos][yPos].time);
	    // if(timeDiffIon > 0){
	    gImplant[xPos][yPos].time = bdecay.pspmt.dytime;
	    gImplant[xPos][yPos].dE1 = bdecay.pid.de1;
	    gImplant[xPos][yPos].tofpin01i2s = bdecay.pid.pin01i2s;
	    //}

	    double tdiff_sega[nsega][xPos][yPos];
	    for(int det = 0; det < nsega; det ++){
	      if(bdecay.sega.ecal[det] > 10){
		for(int xPos = 0; xPos < npspmt; xPos++){
		  for(int yPos = 0; yPos < npspmt; yPos++){
		    // cout
		    // Tdiff condition
		    // Do we need to loop over every single position????
		    // if(det == 15 && gImplant[xg][yg].time > 0){
		    // 	cout<<"sega num: "<<det<<" sega ecal: "<<bdecay.sega.ecal[det]<<"xpos: "<<xPos<<" ypos: "<<yPos<<" implant time: " <<fImplant[xg][yg].time*1.e6<<" sega time: "<<bdecay.sega.time[det]<<endl;
		    // }
		    //tdiff condition
		    tdiff_sega[det][xPos][yPos] = bdecay.sega.time[det] - gImplant[xPos][yPos].time;
		    bdecay.corr.sega_implant_tdiff[det][xPos][yPos] = tdiff_sega[det][xPos][yPos];
		    bdecay.corr.sega_implant_itofpin01i2s[det][xPos][yPos] = gImplant[xPos][yPos].tofpin01i2s;
		    bdecay.corr.sega_implant_idE1[det][xPos][yPos] = gImplant[xPos][yPos].dE1;
		  }
		}
	      }
	    }
	    //LaBr3s
	    double tdiff_labr3[nlabr3][xPos][yPos];
	    //Energy condition
	    for(int det = 0; det < nlabr3; det ++){
	      if(bdecay.labr3.ecal[det] > 10){
		for(int xPos = 0; xPos < npspmt; xPos++){
		  for(int yPos = 0; yPos < npspmt; yPos++){
		    //tdiff_labr3[det][xPos][yPos] = bdecay.labr3.time[det]-fImplant[xPos][yPos].time*1.e6;
		    tdiff_labr3[det][xPos][yPos] = bdecay.labr3.time[det] - gImplant[xPos][yPos].time;
		    bdecay.corr.labr3_implant_tdiff[det][xPos][yPos] = tdiff_labr3[det][xPos][yPos];
		    bdecay.corr.labr3_implant_itofpin01i2s[det][xPos][yPos] = gImplant[xPos][yPos].tofpin01i2s;
		    bdecay.corr.labr3_implant_idE1[det][xPos][yPos] = gImplant[xPos][yPos].dE1;
		  }
		}
	      }
	    }  
	  }//good implant time
      }//good position check
  }//implant (isomer search)
