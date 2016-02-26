// The declarations
        MatrixRowIter inIter(&in);          // iterator for going through matrix by rows
        MatrixRowIter outIter(&out);
        Matrix deltah(V, "deltah");         // error in V (same dim as V)
        Matrix deltao(W, "deltao");         // error in W (same dim as W)
        Matrix a("a");                      // forward hidden layer output vector
        Matrix y("y");                      // forward output vector
//BATCH        Matrix sumh(V, "sumh");             // for the batch version of training
//BATCH        Matrix sumo(W, "sumo");             // for the batch version of training



// The algorithm
        {
//BATCH            sumh.constant(0.0);
//BATCH            sumo.constant(0.0);

            Matrix *t = outIter.rowBegin();
            for (Matrix *i = inIter.rowBegin(); inIter.rowNotEnd(); inIter.rowNext(), outIter.rowNext()) {
                // layer 1
                a = i->dot(V);        // sig(i . v)
                a.map(sigp);
                hidden.insert(a, 0, 0);                // insert into matrix with bias 
                hidden.constantCol(numhidden, -1.0);   // put in bias

                // layer 2
                y = hidden.dot(W);    // sig(hidden . w)  uses a and bias node
                y.map(sigp);

                // output error
                t->sub(y);            // t-y     a row vector
                t->mult(y);           // (t-y)y  a row vector
                y.scalarPreSub(1.0);  // 1-y     a row vector
                deltao = t->mult(y);  // (t-y)y(1-y)  -> change in deltao

                // hidden error
                deltah = hidden;
                deltah.mult(hidden.scalarPreSub(1.0));  // hidden (1 - hidden)
                hidden.scalarPreSub(1.0);               // un-does the (1-hidden) change of hidden matrix (OK, its ugly)
                deltah.mult(deltao.dotT(W));
    
                // update sums in batch mode
//BATCH                sumo.add(hidden.Tdot(deltao));
                W.add(hidden.Tdot(deltao).scalarMult(eta));
                deltah.narrow(deltah.maxCols()-1);  // NOTE: remove the bias off the end
//BATCH                sumh.add(i->Tdot(deltah));
                V.add(i->Tdot(deltah).scalarMult(eta));
            }
            // update V and W matrices
//BATCH            sumh.scalarMult(eta);
//BATCH            V.add(sumh);
//BATCH            sumo.scalarMult(eta);
//BATCH            W.add(sumo);


